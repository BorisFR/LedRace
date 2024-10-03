#ifndef _OLR_COMMON__h
#define _OLR_COMMON__h

#ifdef __cplusplus

extern "C"
{
#endif

    ////////////////////////////////////////////////////////////////////
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
    // !!!!!!!!!!!!!!! DO NOT CHANGE ANYTHING BELOW !!!!!!!!!!!!!!!!! //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
    ////////////////////////////////////////////////////////////////////

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

        MAX_PLAYERS = 4, // DO NOT Change: Current software supports max 4 controllers
    };

#ifdef __cplusplus
} // extern "C"
#endif

#endif
