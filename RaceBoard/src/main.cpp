#include <Arduino.h>
#include "EnumsAndConstants.h"
#include "HardwareConfiguration.h"

/* ***************************************************************************
  Audio stuff
*************************************************************************** */
#include "Audio.h"
Audio audio = Audio();

/* ***************************************************************************
  Game stuff
*************************************************************************** */
#include "GameConfiguration.h"

/* ***************************************************************************
  Led Strip stuff
*************************************************************************** */
#include "LedStrip.h"
LedStrip track = LedStrip();

static uint32_t car_color[] = {
    CAR_COLOR_1,
    CAR_COLOR_2,
    CAR_COLOR_3,
    CAR_COLOR_4};

/* ***************************************************************************
  Controller stuff
*************************************************************************** */
#include "Controller.h"
Controller controller = Controller();

/* ***************************************************************************
  Car stuff
*************************************************************************** */
// #include "Cars.h"
// Cars allCar = Cars();

/* ***************************************************************************
  Telemetry stuff
*************************************************************************** */
#include "Telemetry.h"
Telemetry telemetry = Telemetry();

/*
enum internal_setup
{
  REC_COMMAND_BUFLEN = 32, // received command buffer size
                           // At the moment, the largest received command is RAMP CONFIGURATION (A)
                           //    ex: A1400,1430,1460,12,0[EOC] (for a 1500 LED strip)
                           // 21 CHAR

  TX_COMMAND_BUFLEN = 48, // send command buffer size
                          // At the moment, the largest send command is Q
                          //    ex: QTK:1500,1500,0,-1,60,0,0.006,0.015,1[EOC] (for a 1500 LED strip)
                          // 37 CHAR

  MAX_PLAYERS = MAX_PLAYER_NUMBER, // DO NOT Change: Current software supports max 4 controllers
};
*/

#include "open-led-race.h"

#include <EEPROM.h>
#include "olr-lib.h"
#include "olr-param.h"
#include "SoftTimer.h"
#include "SerialCommand.h"

// A4P -> A = Open LED Race, 4P0 = Game ID (4P = 4 Players, 0=Type 0)
char const softwareId[] = "A2P0";
char const version[] = "1.2.0";

static TheRace race;
static OneCar cars[MAX_PLAYER_NUMBER];
static OneController switchs[MAX_PLAYER_NUMBER];
static TheTrack theTrack;
char tracksID[NUM_TRACKS][2] = {"U", "M", "B", "I", "O"};

static int countdownCurrentPhase = 1;
static bool countdownNewPhase = true;

static int const eeadrInfo = 0;

// non blocking delays
SoftTimer startRaceDelay = SoftTimer();  // Autostart, Countdown
SoftTimer demoModeDelay = SoftTimer();   // Activate Demo Mode on inactivity
SoftTimer telemetryDelay = SoftTimer(0); // Send Telemetry data

/* ESP32 WROOM
Serial 0 : RX0=3 TX0=1 (USB)
Serial 1 : RX1=9 TX1=10
Serial 2 : RX2=16 TX2=17
*/
char cmd[REC_COMMAND_BUFLEN];                                                                 // Stores command received by ReadSerialComand()
SerialCommand serialCommand = SerialCommand(cmd, REC_COMMAND_BUFLEN, EOL, &Serial, &Serial2); // get complete command from serial
char txbuff[TX_COMMAND_BUFLEN];

void debug(String text)
{
  Serial.println("* " + text);
}

void param_load(struct ConfigurationParameter *cfg)
{

  /**
  // Ignore EEPROM params during development of a new version of the [ConfigurationParameter]
  param_setdefault( &theTrack.cfg );
  sprintf( txbuff, "%s%c", "Temporary....DEFAULT PAREMETRS LOADED ", EOL );
  serialCommand.sendCommand(txbuff);
  return;
  **/

  EEPROM.get(eeadrInfo, theTrack.cfg);

  //    sprintf( txbuff, "%s:%d%c", "EEPROM-v", theTrack.cfg.ver, EOL );
  //    serialCommand.sendCommand(txbuff);

  if (theTrack.cfg.ver != CFGPARAM_VER)
  { // [ConfigurationParameter.ver] read form EEPROM != [#define CFGPARAM_VER] in the code
    // Each time a new version of the code modify the [ConfigurationParameter] struct, [#define CFGPARAM_VER] is also
    // changed to force the code enter here.
    // The previous values stored in EEPROM are invalid and need to be reset-to-default and
    // stored in the EEPROM again with the new "structure"
    param_setdefault(&theTrack.cfg, track.NumberOfPixels());
    EEPROM.put(eeadrInfo, theTrack.cfg);
    sprintf(txbuff, "%s%d)%c", "Hello (v", theTrack.cfg.ver, EOL);
    serialCommand.sendCommand(txbuff);
  }
}

/**
 *
 */
void set_controllers_mode(uint8_t numctrl, uint8_t mode)
{
  // debug("set_controllers_mode: " + String(numctrl) + "/" + String(mode));
  for (uint8_t i = 0; i < numctrl; ++i)
  {
    // controller.controller_init(&switchs[i], static_cast<ControllerType>(mode), DIGITAL_CTRL[i]);
    controller.controller_init(&switchs[i], static_cast<ControllerType>(mode), i);
  }
}

/**
 *
 */
void init_cars(uint8_t numcars)
{
  // debug("init_cars: " + String(numcars));
  for (uint8_t i = 0; i < numcars; ++i)
  {
    car_init(&cars[i], &switchs[i], car_color[i]);
  }
}

/*
 *
 */
void draw_ramp(TheTrack *_theTrack)
{
  // debug("draw_ramp");
  struct ConfigurationRamp const *r = &_theTrack->cfg.ramp;
  byte dist = 0;
  byte intensity = 0;
  for (int i = r->init; i <= r->center; ++i)
  {
    dist = r->center - r->init;
    intensity = (32 * (i - r->init)) / dist;
    track.SetPixelColor(i, track.Color(intensity, 0, intensity));
  }
  for (int i = r->center; i <= r->end; ++i)
  {
    dist = r->end - r->center;
    intensity = (32 * (r->end - i)) / dist;
    track.SetPixelColor(i, track.Color(intensity, 0, intensity));
  }
}

/*
 *
 */
void draw_box_entrypoint(TheTrack *_theTrack)
{
  //  debug("draw_box_entrypoint");
  struct ConfigurationTrack const *cfg = &_theTrack->cfg.track;
  int out = cfg->nled_total - cfg->box_len; // Pit lane exit (race start)
  int in = out - cfg->box_len;              // Pit lane Entrance
  track.SetPixelColor(in, COLOR_BOXMARKS);
  track.SetPixelColor(out, COLOR_BOXMARKS);
}

void strip_clear(TheTrack *theTrack, bool show_settings)
{
  // debug("strip_clear");
  struct ConfigurationTrack const *cfg = &theTrack->cfg.track;
  for (int i = 0; i < cfg->nled_main; i++)
    track.SetPixelColor(i, track.Color(0, 0, 0));

  for (int i = 0; i < cfg->nled_aux; i++)
    track.SetPixelColor(cfg->nled_main + i, track.Color(0, 0, 0));

  if (show_settings)
  {
    if (ramp_isactive(theTrack))
      draw_ramp(theTrack);
    if (box_isactive(theTrack))
      draw_box_entrypoint(theTrack);
  }
  // debug("strip_clear OK");
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial2.begin(SERIAL_BAUD);
  delay(1500);
  Serial2.println("Hello LedRace");
  heap_caps_check_integrity_all(true);
  debug("* Led Race * *");
  randomSeed(analogRead(A6) + analogRead(A7));
  telemetry.Setup();
  controller.Setup();
  param_load(&theTrack.cfg);
  track.Setup();
  // First 2 controllers always active (Red, Green)
  race.numcars = 2;
  // Calculate actual players number
  if (controller.controller_isActive(CTRL_3) || param_option_is_active(&theTrack.cfg, PLAYER_3_OPTION) || param_option_is_active(&theTrack.cfg, PLAYER_4_OPTION))
  {
    ++race.numcars;
  }
  if (controller.controller_isActive(CTRL_4) || param_option_is_active(&theTrack.cfg, PLAYER_4_OPTION))
  {
    ++race.numcars;
  }

  // Check if DEMO mode is configured
  race.demo_mode = param_option_is_active(&theTrack.cfg, DEMO_MODE_OPTION);
  enum ControllerType current_mode = (race.demo_mode == true) ? DEMO_MODE : DIGITAL_MODE;

  // !!! Eliminare var current_mode ...mettere if contratto direttamente in f() call

  // Initialize Controllers for very player
  set_controllers_mode(race.numcars, current_mode);

  // Initialize car for every player
  init_cars(race.numcars);

  strip_clear(&theTrack, false);

  // Check Box before Physic/Sound to allow user to have Box and Physics with no sound
  if (controller.controller_isActive(CTRL_2) || param_option_is_active(&theTrack.cfg, BOX_MODE_OPTION))
  { // push switch 2 on reset for activate boxes (pit lane)
    box_init(&theTrack);
    track_configure(&theTrack, theTrack.cfg.track.nled_total - theTrack.cfg.track.box_len);
    draw_box_entrypoint(&theTrack);
  }
  else
  {
    track_configure(&theTrack, 0);
  }

  if (controller.controller_isActive(CTRL_1) || param_option_is_active(&theTrack.cfg, SLOPE_MODE_OPTION))
  { // push switch 1 on reset for activate physics
    ramp_init(&theTrack);
    draw_ramp(&theTrack);
    track.Show();
    delay(2000);
    if (controller.controller_isActive(CTRL_1))
    { // retain push switch  on reset for activate FX sound
      audio.MotorSound(true);
    }
  }
  if (controller.controller_isActive(CTRL_2))
  { // retain push switch on reset for unmute sound
    audio.MuteOff();
  }

  race.network_race = false; // always starts in standalone mode
  race.demo_mode_on_received = false;
  race.demo_mode_off_received = false;

  race.cfg.startline = theTrack.cfg.race.startline;   // always true for Standalone mode
  race.cfg.nlap = theTrack.cfg.race.nlap;             // NUMLAP;
  race.cfg.nrepeat = theTrack.cfg.race.nrepeat;       // always 1 for Standalone mode
  race.cfg.finishline = theTrack.cfg.race.finishline; // always true for Standalone mode

  startRaceDelay.start(0); // first race starts with no delay
  race.phase = READY;      // READY is the first status for Standalone mode

  //  last_activity_millis = millis();

  // debug("SETUP: OK");
}

bool players_actity(uint8_t numcars)
{
  debug("players_actity: " + String(numcars));
  for (uint8_t i = 0; i < numcars; ++i)
  {
    if (controller.controller_isActive(i))
      return (true);
  }
  return (false);
}

/*
 * Check if Controllers (players) were incative for more than INACTIVITY_TIMEOUT_DELAY sec
 */
bool ready_for_demo_mode(void)
{
  debug("ready_for_demo_mode");
  if (players_actity(race.numcars))
  {
    demoModeDelay.start((unsigned long)INACTIVITY_TIMEOUT_DELAY * 1000); // Reset timeout when somebody is using controllers
  }
  return (demoModeDelay.elapsed());
}

/**
 *
 */
void activate_demo_mode(void)
{
  debug("activate_demo_mode");
  race.demo_mode = true;
  race.demo_mode_on_received = false; // reset flag
  set_controllers_mode(race.numcars, DEMO_MODE);
  race.winner = 0; // force a fake winner (used in Status=Complete by draw_winner())
  race.phase = COMPLETE;

  sprintf(txbuff, "%c%d%c", 'M', 1, EOL);
  serialCommand.sendCommand(txbuff);
}

/**
 *
 */
void exit_demo_mode(void)
{
  debug("exit_demo_mode");
  race.demo_mode = false;
  race.demo_mode_off_received = false; // reset flag
  set_controllers_mode(race.numcars, DIGITAL_MODE);
  race.winner = 0; // force a fake winner (used in Status=Complete by draw_winner())
  race.phase = COMPLETE;

  sprintf(txbuff, "%c%d%c", 'M', 0, EOL);
  serialCommand.sendCommand(txbuff);
}

/*
 *  reset race parameters
 *  stop sound
 */
void enter_configuration_mode()
{
  debug("enter_configuration_mode");
  audio.SoundOff();
  strip_clear(&theTrack, false);
  track.Show();
}

/*
 *  Display on LED Strip current values for Slope and Pitlane
 *
 */
void show_cfgpars_onstrip()
{
  debug("show_cfgpars_onstrip");
  strip_clear(&theTrack, true);
  track.Show();
}

/*
 *  Send Log/Warning/Error messages to host
 */
void printdebug(const char *msg, int errlevel)
{
  char header[5];
  sprintf(header, "!%d,", errlevel);
  debug(String(header) + ": " + String(msg));
  // Serial.print(header);
  // Serial.print(msg);
  // Serial.print(EOL);
}

/*
 *  Check Serial to see if there is a command ready to be processed
 *
 */
ack_t manageSerialCommand()
{
  // debug("manageSerialCommand");
  ack_t ack = {.rp = NOTHING, .type = '\0'};

  int clen = serialCommand.checkSerial();
  if (clen == 0)
    return ack; // No commands received

  // debug("manageSerialCommand: received");
  if (clen < 0)
  { // Error receiving command
    sprintf(txbuff, "Error reading serial command:[%d]", clen);
    printdebug(txbuff, WARNING);
  }

  // clen > 0 ---> Command with length=clen ready in  cmd[]
  ack.rp = NOK;

  // Debug only
  // if(race.network_race == true) {
  //  sprintf( txbuff, "Recv cmd:%s", cmd);
  //  printdebug( txbuff, WARNING );
  //}

  switch (cmd[0])
  {

  case '#': // Handshake
  {
    ack.type = cmd[0];
    sprintf(txbuff, "#%c", EOL);
    serialCommand.sendCommand(txbuff);
    ack.rp = NOTHING;
  }
  break;

  case '@': // Enter "Configuration Mode" status
  {
    ack.type = cmd[0];
    if (race.phase != CONFIG)
    { // Ignore command if Board already in "Configure Mode"
      race.phase = CONFIG;
      enter_configuration_mode();
    }
    ack.rp = OKK;
  }
  break;

  case '*': // Exit "Configure Mode"
  {
    ack.type = cmd[0];
    if (race.phase == CONFIG)
    { // Ignore command if Board is not in "Configure Mode"
      race.newcfg = true;
    }
    ack.rp = OKK;
  }
  break;

  case 'R': // Set Race Phase
  {
    ack.type = cmd[0];
    uint8_t const phase = atoi(cmd + 1);
    /**
    *  Reintrodotto momentaneamente R1=enter config per testare RelayRace con Networkclient esistente
    // Does not accept anymore  R=1 as Enter Configuration / Use command @ instead
    if( 0 > phase || RACE_PHASES <= phase || phase == CONFIG) return ack;
    race.phase = (enum phases) phase;
    ack.rp = OKK;
    **/
    // Codice vecchio con R1=Enter Configuration
    if (0 > phase || RACE_PHASES <= phase)
      return ack;
    race.phase = (enum phases)phase;
    ack.rp = OKK;
    if (race.phase == CONFIG)
    { // accept R1 as a EnterConfigurationMode command - DEPRECATED
      enter_configuration_mode();
    }
  }
  break;

  case 'u': // Car Enter the Circuit - // OLR Network only
  {
    ack.type = cmd[0];
    byte const data = cmd[1];
    byte const ncar = 0b00000111 & (data >> 5);
    byte const speed = 0b00011111 & data;
    if (0 >= ncar || race.numcars < ncar)
      return ack;
    cars[ncar - 1].st = CAR_ENTER;
    cars[ncar - 1].speed = (float)speed / 10;
    ack.rp = OKK;
    ack.rp = NOTHING;
    if (false)
    {
      sprintf(txbuff, "%s %d, %s %d, %s %d", "CAR: ", ncar, "STATUS: ", cars[ncar - 1].st, "SPEED: ", (int)(cars[ncar - 1].speed * 10));
      printdebug(txbuff, LOG);
    }
  }
  break;

  case 't': // Car Coming into the Circuit - // OLR Network only
  {
    ack.type = cmd[0];
    byte const ncar = atoi(cmd + 1);
    if (0 >= ncar || race.numcars < ncar)
      return ack;
    cars[ncar - 1].st = CAR_COMING;
    ack.rp = OKK;
    if (false)
    {
      sprintf(txbuff, "%s %d, %s %d", "CAR: ", ncar, "STATUS: ", cars[ncar - 1].st);
      printdebug(txbuff, LOG);
    }
  }
  break;

  case 'w': // Car Wins the race - OLR Network only
            // Standalone mode: Board _NEVER_ receives a 'w' command !!!
  {         // Network mode: 1) Command "w" sent by the Board where the race ends
            //               2) Every other participant (Board) receives 'w' command
    ack.type = cmd[0];
    byte const ncar = atoi(cmd + 1);
    if (0 >= ncar || race.numcars < ncar)
      return ack;
    if (race.network_race && (!race.cfg.finishline))
    {
      race.winner = (byte)ncar - 1; // Set the Winner
    }
    ack.rp = NOTHING;
  }
  break;

  case 'C': // Parse race configuration -> C1,2,3,0
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "C");
    if (!pch)
      return ack;

    pch = strtok(pch, ",");
    if (!pch)
      return ack;
    int startline = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int nlap = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int nrepeat = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int finishline = atoi(pch);

    int err = race_configure(&theTrack, startline, nlap, nrepeat, finishline);
    if (err)
      return ack;

    race.cfg.startline = theTrack.cfg.race.startline;
    race.cfg.nlap = theTrack.cfg.race.nlap;
    race.cfg.nrepeat = theTrack.cfg.race.nrepeat;
    race.cfg.finishline = theTrack.cfg.race.finishline;

    ack.rp = OKK;
  }
  break;

  case 'T': // Parse Track configuration -> Track length
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "T");
    if (!pch)
      return ack;

    int nled = atoi(cmd + 1);
    int err = tracklen_configure(&theTrack, nled);
    if (err)
      return ack;
    track_configure(&theTrack, 0);
    if (err)
      return ack;

    ack.rp = OKK;
  }
  break;

  case 'B': // Parse BoxLenght Configuration -> Blen,perm
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "B");
    if (!pch)
      return ack;

    pch = strtok(pch, ",");
    if (!pch)
      return ack;
    int boxlen = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int boxperm = atoi(pch);

    int err = boxlen_configure(&theTrack, boxlen, boxperm);
    if (err)
      return ack;

    ack.rp = OKK;

    // Force Pitlane ON, so "show_cfgpars_onstrip()"
    // will show the new values, even if AlwaysON=false
    box_init(&theTrack);
    show_cfgpars_onstrip();
  }
  break;

  case 'A': // Parse Ramp configuration -> Astart,center,end,high,perm
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "A");
    if (!pch)
      return ack;

    pch = strtok(pch, ",");
    if (!pch)
      return ack;
    int init = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int center = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int end = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int high = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    int slopeperm = atoi(pch);

    uint8_t err = ramp_configure(&theTrack, init, center, end, high, slopeperm);
    if (err)
      return ack;
    ack.rp = OKK;

    // Force Ramp ON, so "show_cfgpars_onstrip()"
    // will show the new values, even if AlwaysON=false
    ramp_init(&theTrack);

    show_cfgpars_onstrip();
  }
  break;

  case 'E': // Parse Battery configuration -> Edelta,min,boost,active
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "E");
    if (!pch)
      return ack;

    pch = strtok(pch, ",");
    if (!pch)
      return ack;
    uint8_t delta = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    uint8_t min = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    uint8_t boost = atoi(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    uint8_t active = atoi(pch);

    uint8_t err = battery_configure(&theTrack, delta, min, boost, active);
    if (err)
      return ack;
    ack.rp = OKK;
  }
  break;

  case 'G': // Parse Autostart configuration -> Gautostart
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "G");
    if (!pch)
      return ack;

    uint8_t autostart = atoi(cmd + 1);
    uint8_t err = autostart_configure(&theTrack, autostart);
    if (err)
      return ack;

    ack.rp = OKK;
  }
  break;

  case 'M': // Parse DEMO mode configuration
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "M");
    if (!pch)
      return ack;

    uint8_t demo = atoi(cmd + 1);

    uint8_t err = demo_configure(&theTrack, demo);
    if (err)
      return ack;
    ack.rp = OKK;

    if (demo == 0)
    {
      race.demo_mode_off_received = true;
    }
    else if (race.demo_mode)
    {
      race.demo_mode_on_received = true;
    }
  }
  break;

  case 'P': // Parse Player 3/4 configuration -> P[2|3|4]
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "P");
    if (!pch)
      return ack;

    uint8_t players_n = atoi(cmd + 1);
    uint8_t err = players_n_configure(&theTrack, players_n);
    if (err)
      return ack;

    ack.rp = OKK;
  }
  break;

  case 'K': // Parse Physic simulation parameters
  {
    ack.type = cmd[0];

    char *pch = strtok(cmd, "K");
    if (!pch)
      return ack;

    pch = strtok(pch, ",");
    if (!pch)
      return ack;
    float kgp = atof(pch);

    pch = strtok(NULL, ",");
    if (!pch)
      return ack;
    float kfp = atof(pch);

    int err = physic_configure(&theTrack, kgp, kfp);
    if (err)
      return ack;

    ack.rp = OKK;
  }
  break;

  case 'H': // Tunnel configuration - // OLR Network only
  {
    ack.type = cmd[0];
    uint8_t const dtunnel = atoi(cmd + 1);
    if (0 >= dtunnel || 254 < dtunnel)
      return ack;
    race.circ.outtunnel = dtunnel;
    ack.rp = OKK;
    if (false)
    { // VERBOSE
      sprintf(txbuff, "%s %d", "TUNNEL: ", race.circ.outtunnel);
      printdebug(txbuff, LOG);
    }
  }
  break;

  case 'D': // Load Default Parameters and store them in from EEPROM
  {
    ack.type = cmd[0];
    param_setdefault(&theTrack.cfg, track.NumberOfPixels());
    EEPROM.put(eeadrInfo, theTrack.cfg); // Save immediately

    ack.rp = OKK;

    // Update box/slope active in current Track Struct with values
    // just loaded (for show_cfgpars_onstrip())
    struct ConfigurationParameter const *cfg = &theTrack.cfg;
    theTrack.boxactive = param_option_is_active(&theTrack.cfg, BOX_MODE_OPTION);
    theTrack.rampactive = param_option_is_active(&theTrack.cfg, SLOPE_MODE_OPTION);

    show_cfgpars_onstrip();
  }
  break;

  case ':': // Set board Unique Id
  {
    struct BoardInfo *info = &theTrack.cfg.info;
    ack.type = cmd[0];
    if (strlen(cmd + 1) > LEN_UID)
      return ack;
    strcpy(info->uid, cmd + 1);
    EEPROM.put(eeadrInfo, theTrack.cfg); // Save immediately
    ack.rp = OKK;
  }
  break;

  case '$': // Get Board UID
  {
    sprintf(txbuff, "%s%s%c", "$", theTrack.cfg.info.uid, EOL);
    serialCommand.sendCommand(txbuff);
    ack.rp = NOTHING;
  }
  break;

  case '?': // Get Software Id
  {
    sprintf(txbuff, "%s%s%c", "?", softwareId, EOL);
    serialCommand.sendCommand(txbuff);
    ack.rp = NOTHING;
  }
  break;

  case '%': // Get Software Version
  {
    sprintf(txbuff, "%s%s%c", "%", version, EOL);
    serialCommand.sendCommand(txbuff);
    ack.rp = NOTHING;
  }
  break;

  case 'n': // Set "Network Race" mode (Relay race)
  {
    ack.type = cmd[0];
    race.network_race = true;
    race.phase = COMPLETE; // Immediatly ends the current race (if any)
    race.winner = 0;       // Set a fake winner (used in Status=Complete by draw_winner())
    ack.rp = OKK;
  }
  break;

  case 'Q': // Get current configuration Info
  {
    struct ConfigurationParameter const *cfg = &theTrack.cfg;
    sprintf(txbuff, "%s:%d,%d,%d,%d,%d,%d,%d.%03d,%d.%03d,%d%c", "QTK",
            cfg->track.nled_total,
            cfg->track.nled_main,
            cfg->track.nled_aux,
            cfg->track.init_aux,
            cfg->track.box_len,
            // cfg->track.box_alwaysOn,
            param_option_is_active(&theTrack.cfg, BOX_MODE_OPTION),
            (int)cfg->track.kg, (int)(cfg->track.kg * 1000) % 1000, // std arduino sprintf() missing %f
            (int)cfg->track.kf, (int)(cfg->track.kf * 1000) % 1000, // std arduino sprintf() missing %f
            param_option_is_active(&theTrack.cfg, AUTOSTART_MODE_OPTION),
            EOL);
    serialCommand.sendCommand(txbuff);

    sprintf(txbuff, "%s:%d,%d,%d,%d,%d%c", "QRP",
            cfg->ramp.init,
            cfg->ramp.center,
            cfg->ramp.end,
            cfg->ramp.high,
            // cfg->ramp.alwaysOn,
            param_option_is_active(&theTrack.cfg, SLOPE_MODE_OPTION),
            EOL);
    serialCommand.sendCommand(txbuff);

    sprintf(txbuff, "%s:%d,%d,%d,%d%c", "QBT",
            cfg->battery.delta,
            cfg->battery.min,
            cfg->battery.speed_boost_scaler,
            param_option_is_active(&theTrack.cfg, BATTERY_MODE_OPTION),
            EOL);
    serialCommand.sendCommand(txbuff);

    sprintf(txbuff, "%s:%d,%d,%d,%d,%d,%d,%d,%d%c", "QRC",
            cfg->race.startline,
            cfg->race.nlap,
            cfg->race.nrepeat,
            cfg->race.finishline,
            param_option_is_active(&theTrack.cfg, PLAYER_3_OPTION),
            param_option_is_active(&theTrack.cfg, PLAYER_4_OPTION),
            param_option_is_active(&theTrack.cfg, DEMO_MODE_OPTION),
            // race.demo_mode,
            race.network_race,
            EOL);
    serialCommand.sendCommand(txbuff);

    ack.rp = NOTHING;
  }
  break;

  case 'W': // Write configuration to EEPROM
  {
    ack.type = cmd[0];
    EEPROM.put(eeadrInfo, theTrack.cfg);
    ack.rp = OKK;
  }
  break;

  } // switch

  // debug("manageSerialCommand: DONE");
  return (ack);
}

/*
 *
 */
void sendResponse(ack_t *ack)
{
  // debug("sendResponse");
  if (ack->type == '\0')
  {
    sprintf(txbuff, "%s%c", ack->rp == OKK ? "OK" : "NOK", EOL);
  }
  else
  {
    sprintf(txbuff, "%c%s%c", ack->type, ack->rp == OKK ? "OK" : "NOK", EOL);
  }
  serialCommand.sendCommand(txbuff);
}

/*
 *
 */
void countdownReset()
{
  // debug("countdownReset");
  countdownCurrentPhase = 1;
  countdownNewPhase = true;
}

void send_phase(int phase)
{
  // debug("send_phase");
  sprintf(txbuff, "R%d%c", phase, EOL);
  serialCommand.sendCommand(txbuff);
}

/*
 *  non-blocking
 */
boolean start_race_done()
{
  // debug("start_race_done");
  if (countdownNewPhase)
  {
    // debug("start_race_done: new phase");
    countdownNewPhase = false;
    startRaceDelay.start(CONTDOWN_PHASE_DURATION);
    strip_clear(&theTrack, true);
    switch (countdownCurrentPhase)
    {
    case 1:
      // debug("start_race_done: 1");
      audio.PlayCountdown((countdown)countdownCurrentPhase);
      track.SetPixelColor(LED_SEMAPHORE, track.Color(255, 0, 0));
      break;
    case 2:
      // debug("start_race_done: 2");
      audio.PlayCountdown((countdown)countdownCurrentPhase);
      track.SetPixelColor(LED_SEMAPHORE, track.Color(0, 0, 0));
      track.SetPixelColor(LED_SEMAPHORE - 1, track.Color(255, 255, 0));
      break;
    case 3:
      // debug("start_race_done: 3");
      audio.PlayCountdown((countdown)countdownCurrentPhase);
      track.SetPixelColor(LED_SEMAPHORE - 1, track.Color(0, 0, 0));
      track.SetPixelColor(LED_SEMAPHORE - 2, track.Color(0, 255, 0));
      break;
    case 4:
      // debug("start_race_done: 4");
      startRaceDelay.start(CONTDOWN_STARTSOUND_DURATION);
      audio.PlayCountdown((countdown)countdownCurrentPhase);
      track.SetPixelColor(LED_SEMAPHORE - 2, track.Color(0, 0, 0));
      track.SetPixelColor(0, track.Color(255, 255, 255));
      break;
    case 5:
      // debug("start_race_done: 5");
      audio.PlayCountdown((countdown)countdownCurrentPhase);
      countdownReset(); // reset for next countdown
      return (true);
    }
    track.Show();
  }
  if (startRaceDelay.elapsed())
  {
    // debug("start_race_done: elapsed");
    audio.SoundOff();
    countdownNewPhase = true;
    countdownCurrentPhase++;
  }
  return (false);
}

void draw_coin(TheTrack *theTrack)
{
  // debug("draw_coin");
  struct ConfigurationTrack const *cfg = &theTrack->cfg.track;
  track.SetPixelColor(1 + cfg->nled_main + cfg->nled_aux - theTrack->ledcoin, COLOR_COIN);
}

void draw_winner(TheTrack *theTrack, uint32_t color)
{
  // debug("draw_winner");
  struct ConfigurationTrack const *cfg = &theTrack->cfg.track;
  for (int i = 16; i < cfg->nled_main; i = i + (8 * cfg->nled_main / 300))
  {
    track.SetPixelColor(i, color);
    track.SetPixelColor(i - 16, 0);
    track.Show();
    delay(WINNER_SHOW_DELAY);
  }
  delay(WINNER_SHOW_DURATION);
}

void draw_car_tail(TheTrack *theTrack, OneCar *car)
{
  // debug("draw_car_tail");
  struct ConfigurationTrack const *cfg = &theTrack->cfg.track;

  switch (car->trackID)
  {
  case TRACK_MAIN:
    for (int i = 0; i <= car->nlap; ++i)
      track.SetPixelColor(((word)car->dist % cfg->nled_main) + i, car->color);
    break;
  case TRACK_AUX:
    for (int i = 0; i <= car->nlap; ++i)
      track.SetPixelColor((word)(cfg->nled_main + cfg->nled_aux - car->dist_aux) + i, car->color);
    break;
  }
}

void sound_winner(TheTrack *theTrack, byte winner)
{
  //  debug("sound_winner");
  audio.PlayWinnerMusic();
}

void draw_car(TheTrack *theTrack, OneCar *car)
{
  // debug("draw_car");
  struct ConfigurationTrack const *cfg = &theTrack->cfg.track;
  struct ConfigurationBattery const *battery = &theTrack->cfg.battery;

  switch (car->trackID)
  {
  case TRACK_MAIN:
    for (int i = 0; i <= 1; ++i)
      track.SetPixelColor(((word)car->dist % cfg->nled_main) - i, car->color);
    //        uint8_t carlenght = car->nlap < 6 ? car->nlap : 6;  // Max cartatail lenght = 7
    //        for(int i=0; i<= carlenght; ++i )
    //          track.setPixelColor( ((word)car->dist % cfg->nled_main) + i, car->color );
    if (param_option_is_active(&theTrack->cfg, BATTERY_MODE_OPTION))
    { // Battery Mode ON
      if (car->charging == 1)
      {
        track.SetPixelColor(((word)car->dist % cfg->nled_main) - 2, 0x010100 * 50 * (millis() / (201 - 2 * (byte)car->battery) % 2));
      }
      else if (car->battery <= battery->min)
        if ((millis() % 100) > 50)
          track.SetPixelColor(((word)car->dist % cfg->nled_main) - 2, WARNING_BLINK_COLOR);
    }
    break;
  case TRACK_AUX:
    for (int i = 0; i <= 1; ++i)
      track.SetPixelColor((word)(cfg->nled_main + cfg->nled_aux - car->dist_aux) + i, car->color);
    if (param_option_is_active(&theTrack->cfg, BATTERY_MODE_OPTION))
    { // Battery Mode ON

      if (car->charging == 1)
      {
        track.SetPixelColor((word)(cfg->nled_main + cfg->nled_aux - car->dist_aux) + 2, 0x010100 * 50 * (millis() / (201 - 2 * (byte)car->battery) % 2));
      }
      else if (car->battery <= battery->min)
        if ((millis() % 100) > 50)
          track.SetPixelColor((word)(cfg->nled_main + cfg->nled_aux - car->dist_aux) + 2, WARNING_BLINK_COLOR);
    }
    break;
  }
}

void run_racecycle(OneCar *car, int caridx)
{
  // debug("run_racecycle");
  struct ConfigurationTrack const *cfg = &theTrack.cfg.track;

  // if( car->st == CAR_COMING ) {  // OLR Network only
  //   // To be implemented
  // }

  if (car->st == CAR_ENTER)
  {
    // Standalone mode => On Race start the Speed get RESET (speed=0)
    // Network race    => Car speed set when receiving the Car_Enter Serial command
    (race.network_race == true) ? car_resetPosition(car, false) : car_resetPosition(car, true);

    if (car->repeats < race.cfg.nrepeat)
      car->st = CAR_RACING;
    else
      car->st = CAR_GO_OUT;
  }

  if (car->st == CAR_RACING)
  {
    update_track(&theTrack, car, controller.controller_getStatus(car->ct), controller.controller_getAccel());
    car_updateController(car, controller.controller_getSpeed(car->ct));
    draw_car(&theTrack, car);

    if (car->nlap == race.cfg.nlap && !car->leaving && car->dist > (cfg->nled_main * car->nlap - race.circ.outtunnel))
    {
      car->leaving = true;
      car->st = CAR_LEAVING;
    }

    if (car->nlap > race.cfg.nlap)
    {
      ++car->repeats;
      car->st = CAR_GO_OUT;
    }

    if (car->repeats >= race.cfg.nrepeat && race.cfg.finishline)
    {
      car->st = CAR_FINISH;
    }
  }

  if (car->st == CAR_LEAVING)
  { // OLR Network only
    car->st = CAR_RACING;
    sprintf(txbuff, "r%d%c", caridx + 1, EOL);
    serialCommand.sendCommand(txbuff);
  }

  if (car->st == CAR_GO_OUT)
  { // OLR Network only
    car->st = CAR_WAITING;
    // map car number in 3 higher bits and car speed in 5 lower bits
    byte const speed = car->speed * 10;
    byte const data = (caridx + 1) << 5 | (0b00011111 & speed);
    sprintf(txbuff, "s%c%c", data, EOL);
    serialCommand.sendCommand(txbuff);
    ;
    car_resetPosition(car, true);
    car->trackID = NOT_TRACK;
  }

  if (car->st == CAR_FINISH)
  {
    car->trackID = NOT_TRACK;
    sprintf(txbuff, "w%d%c", caridx + 1, EOL);
    serialCommand.sendCommand(txbuff);

    car_resetPosition(car, true);
  }
}

int get_relative_position(OneCar *car)
{
  // debug("get_relative_position");
  enum
  {
    MIN_RPOS = 0,
    MAX_RPOS = 99,
  };
  struct ConfigurationTrack const *cfg = &theTrack.cfg.track;
  int trackdist = 0;
  int pos = 0;

  switch (car->trackID)
  {
  case TRACK_MAIN:
    trackdist = (int)car->dist % cfg->nled_main;
    pos = map(trackdist, 0, cfg->nled_main - 1, MIN_RPOS, MAX_RPOS);
    break;
  case TRACK_AUX:
    trackdist = (int)car->dist_aux;
    pos = map(trackdist, 0, cfg->nled_aux - 1, MIN_RPOS, MAX_RPOS);
    break;
  }
  return pos;
}

void print_cars_positions(OneCar *cars)
{
  // debug("print_cars_positions");
  bool outallcar = true;
  for (int i = 0; i < race.numcars; ++i)
    outallcar &= cars[i].st == CAR_WAITING;

  if (outallcar)
    return;

  for (int i = 0; i < race.numcars; ++i)
  {
    int const rpos = get_relative_position(&cars[i]);
    sprintf(txbuff, "p%d%s%d,%d,%d%c", i + 1, tracksID[cars[i].trackID], cars[i].nlap, rpos, (int)cars[i].battery, EOL);
    serialCommand.sendCommand(txbuff);
    telemetry.Send(txbuff);
  }
}

void loop()
{
  // look for commands received on serial
  ack_t ack = manageSerialCommand();
  if (ack.rp != NOTHING)
  {
    // debug("ack.rp != NOTHING");
    sendResponse(&ack);
  }

  // Exit DEMO mode when a Player touch a controller
  if (race.demo_mode_off_received || (race.demo_mode && players_actity(race.numcars)))
  {
    debug("time to exit_demo_mode");
    exit_demo_mode();
  }
  // If demo_mode option is set in board configuration
  //    -> Enter demo mode after INACTIVITY_TIMEOUT_DELAY sec
  if (race.demo_mode_on_received || (param_option_is_active(&theTrack.cfg, DEMO_MODE_OPTION) && race.demo_mode == false && ready_for_demo_mode()))
  {
    debug("time to activate_demo_mode");
    activate_demo_mode();
  }

  // PLEASE NOTE:
  //  DO NOT call  "track.show()" in the loop() while in configuration mode !!!
  //  It would mess up with Serial communication (receives only 2 bytes - if the
  //  string sent by the host is longer, it gets lost)
  //  In other phases (READY, RACING, etc) ONLY 2 bytes are guaranteed to be
  //  succesfully received - So "Enter Configuration Mode" command is just one byte (@)

  switch (race.phase)
  {

  case CONFIG:
  {
    debug("PHASE: config");
    if (race.newcfg)
    { // Exit_Config command received
      race.newcfg = false;
      countdownReset();
      startRaceDelay.start(0);
      // for Standalone mode, gets into READY status
      // for Network races gets into CONFIGURATION OK status
      race.phase = (race.network_race == false) ? READY : CONFIG_OK;
      send_phase(race.phase);
    }
  }
  break;

  case CONFIG_OK: // OLR Network only
  {
    debug("PHASE: config ok");
    ; // In a Relay Race the configuration is sent (via 'C' command) by the
      // Host ("Nerwork Client" program running on another Computer)
      // When the board reach the CONFIG_OK status...it does nothing but wait for
      // the next Command coming form the Host.
      // Same thing for the IDLE status (reached at the end of a relay race)
      // In other words, in Network mode (Relay races), some Status changes comes from the Host
  }
  break;

  case READY:
  {
    // debug("PHASE: ready");
    bool goOn = false;

    if (race.cfg.startline)
    { // Standalone: Always true - Network mode: only racetrack where race starts
      // debug("PHASE: ready / startline");

      if (param_option_is_active(&theTrack.cfg, AUTOSTART_MODE_OPTION) || race.demo_mode)
      { // Autostart parameters ON
        debug("PHASE: ready / Autostart");
        if (startRaceDelay.elapsed())
          goOn = true; // Automatically start Countdown after a defined Delay
                       // Note: In DemoMode always use AutoStart
      }
      else
      { // Autostart OFF:
        // debug("PHASE: ready / Not Autostart");
        int pstart = 0; // Wait for every controller be active (button pressed) to start Countdown
        strip_clear(&theTrack, true);
        for (int i = 0; i < race.numcars; ++i)
        {
          // debug("PHASE: ready / Not Autostart : numcars=" + String(i));
          if (controller.controller_getStatus(cars[i].ct) == false)
          {
            // debug("PHASE: ready / Not Autostart => false");
            track.SetPixelColor(i, cars[i].color);
            pstart++;
          }
        }
        // debug("PHASE: ready / Not Autostart / set LED_SEMAPHORE");
        track.SetPixelColor(LED_SEMAPHORE, ((millis() / 5) % 64) * 0x010100);
        // debug("PHASE: ready / Not Autostart / show");
        track.Show();
        // if every controller activated -> Ready for Countdown
        if (pstart == race.numcars)
        {
          // debug("PHASE: ready / Not Autostart / goOn");
          goOn = true;
        }
        // debug("PHASE: ready / Not Autostart end");
      };
    }

    if (goOn || (!race.cfg.startline))
    { // Standalone mode is Ready for Countdown __OR__ Network mode and Race does not starts here
      // debug("PHASE: ready / for countdown");
      for (int i = 0; i < race.numcars; ++i)
      {
        car_resetPosition(&cars[i], true);
        cars[i].repeats = 0;
        cars[i].st = CAR_WAITING; // Network race -> cleanup status of previous race
      }
      theTrack.ledcoin = COIN_RESET;
      race.phase = COUNTDOWN;
      if (race.network_race != true)
        send_phase(race.phase);

      srand((unsigned long)analogRead(A6) + analogRead(A7)); // used in demo_mode (see olr_controllers.h)
    }
    // debug("PHASE: ready end");
  }
  break;

  case COUNTDOWN:
  {
    // debug("PHASE: countdown");
    if (race.cfg.startline)
    { // Standalone: Always true - Network mode: only racetrack where race starts
      // Countdown: semaphore and tones
      if (start_race_done())
      { // Countdown done
        for (int i = 0; i < race.numcars; ++i)
        {
          cars[i].st = CAR_ENTER;
        }
        race.phase = RACING;
        send_phase(race.phase);
      }
    }
  }
  break;

  case RACING:
  {
    // debug("PHASE: racing");
    strip_clear(&theTrack, true);

    if (box_isactive(&theTrack))
    {
      if (theTrack.ledcoin == COIN_RESET)
      {
        theTrack.ledcoin = COIN_WAIT;
        theTrack.ledtime = millis() + random(2000, 7000);
      }
      if (theTrack.ledcoin > 0)
        draw_coin(&theTrack);
      else if (millis() > theTrack.ledtime)
        theTrack.ledcoin = random(20, theTrack.cfg.track.nled_aux - 20);
    }
    else
    {
      if (param_option_is_active(&theTrack.cfg, BATTERY_MODE_OPTION))
      { // Battery Mode ON
        if (theTrack.ledcoin == COIN_RESET)
        {
          theTrack.ledcoin = COIN_WAIT;
          theTrack.ledtime = millis() + random(3000, 8000);
        }
        if (theTrack.ledcoin > 0)
          draw_coin(&theTrack);
        else if (millis() > theTrack.ledtime)
          theTrack.ledcoin = random(LED_SEMAPHORE + 4, theTrack.cfg.track.nled_main - 60); // valid zone from random charge (semaphore to 1 meter before to start-finish position
      }
    }

    for (int i = 0; i < race.numcars; ++i)
    {
      run_racecycle(&cars[i], i);
      if (cars[i].st == CAR_FINISH)
      {
        race.phase = COMPLETE;
        race.winner = (byte)i;
        send_phase(race.phase);
        break;
      }
    }

    track.Show();
    audio.PlayMotorSound(int(cars[0].speed * 440 * 1) + int(cars[1].speed * 440 * 2) + int(cars[2].speed * 440 * 3) + int(cars[3].speed * 440 * 4));

    // Send Telemetry data
    if (telemetryDelay.elapsed())
    {
      print_cars_positions(cars);
      telemetryDelay.start(TELEMETRY_DELAY);
    }
    // ----------------
  }
  break;

  case COMPLETE:
  {
    // debug("PHASE: complete");
    strip_clear(&theTrack, false);
    track.Show();
    draw_winner(&theTrack, cars[race.winner].color);
    sound_winner(&theTrack, race.winner);
    strip_clear(&theTrack, false);
    track.Show();

    startRaceDelay.start(NEWRACE_DELAY);

    // for Standalone mode, gets into READY status
    // for Network races gets into IDLE statue
    race.phase = (race.network_race == false) ? READY : IDLE;
  }
  break;

  case IDLE: // OLR Network only
  {
    // debug("PHASE: idle");
    ; // -- see comment in CONFIG_OK status
  }
  break;

  default:
  {
    debug("PHASE: default, error");
    sprintf(txbuff, "SwErr-01");
    printdebug(txbuff, WARNING);
    break;
  }

  } // switch race.phase
}
