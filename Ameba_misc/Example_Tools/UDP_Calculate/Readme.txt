This is the guide of MacOS, Linux or Windows OS user to compile the tool of the 3 UDP calculate examples.

Pre-request
    For Windows OS user, 
        Please download and install CYGWIN from http://www.cygwin.com/
        While installing chose "g++" at the selection

Compile the tools
    Open the terminal(cygwin terminal for Windows user), then apply the following commands.

    ### g++.exe SendDelay.cpp -lws2_32 -o SendDelay_win.exe
    ### strip SendDelay_win.exe
    g++.exe SendDelay.cpp ../../Ameba_Icon/ico-out.o -o SendDelay_win.exe
    g++ SendDelay.cpp -o SendDelay_linux
    g++ SendDelay.cpp -o SendDelay_mac

    ### g++.exe ReceiveDelay.cpp -lws2_32 -o ReceiveDelay_win.exe
    ### strip ReceiveDelay_win.exe
    g++.exe ReceiveDelay.cpp ../../Ameba_Icon/ico-out.o -o ReceiveDelay_win.exe
    g++ ReceiveDelay.cpp -o ReceiveDelay_linux
    g++ ReceiveDelay.cpp -o ReceiveDelay_mac

    ### g++.exe ReceiveTimeout.cpp -lws2_32 -o ReceiveTimeout_win.exe
    ### strip ReceiveTimeout_win.exe
    g++.exe ReceiveTimeout.cpp ../../Ameba_Icon/ico-out.o -o ReceiveTimeout_win.exe
    g++ ReceiveTimeout.cpp -o ReceiveTimeout_linux
    g++ ReceiveTimeout.cpp -o ReceiveTimeout_mac

Use the tools
    Open the terminal(cygwin terminal for Windows user), then apply the following commands.

    ./ReceiveTimeout_win.exe <client IP address>
    ./ReceiveDelay_win.exe <client IP address>
    ./SendDelay_win.exe

    ./ReceiveDelay_linux <client IP address>
    ./ReceiveTimeout_linux <client IP address>
    ./SendDelay_linux

    ./ReceiveDelay_mac <client IP address>
    ./ReceiveTimeout_mac <client IP address>
    ./SendDelay_mac
