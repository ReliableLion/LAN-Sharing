set(TARGET LAN_Sharing)


add_executable(LAN_Sharing
        Connection.cpp
        Discovery.cpp
        DiscoveryMain.cpp
        DownloadManager.cpp
        FileHandler.cpp
        Message.cpp
        PacketManager.cpp
        Protocol.cpp
        RequestHandler.cpp
        RequestManager.cpp
        Server.cpp
        SocketBuffer.cpp
        UDPService.cpp
        UploadManager.cpp
        UserConfiguration.cpp
        WindowsFileHandler.cpp ProtocolMessageTest.cpp ../include/ProtocolMessageTest.hpp)