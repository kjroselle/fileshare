========================================================================
    CONSOLE APPLICATION : winsock_server Project Overview
========================================================================

Server listens for connections on port 27015. Must set DEFAULT_IP to the local machine's IP address.
When a client connects and sends the string "file request" the server reads the file to be sent into
"send_block" and sends the file to the client.

========================================================================
AppWizard has created this winsock_server application for you.

This file contains a summary of what you will find in each of the files that
make up your winsock_server application.


winsock_server.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

winsock_server.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

winsock_server.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named winsock_server.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
