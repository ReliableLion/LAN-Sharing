cmake_minimum_required(VERSION 3.10)
project(LAN_Sharing)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(LAN_Sharing
        ConcurrentQueue.hpp
        Connection.cpp
        Connection.hpp
        Constants.hpp
        Discovery.cpp
        Discovery.hpp
        DiscoveryMain.cpp
        DownloadManager.cpp
        DownloadManager.hpp
        Exceptions.hpp
        FileHandler.cpp
        FileHandler.hpp
        Listen_socket.cpp
        Listen_socket.hpp
        Message.cpp
        Message.hpp
        PacketManager.cpp
        PacketManager.hpp
        Protocol.cpp
        Protocol.hpp
        RequestHandler.cpp
        RequestHandler.hpp
        RequestManager.cpp
        RequestManager.hpp
        Server.cpp
        Server.hpp
        ServerException.hpp
        SocketBuffer.cpp
        SocketBuffer.hpp
        stdafx.h
        UDPService.cpp
        UDPService.hpp
        UploadManager.cpp
        UploadManager.hpp
        UserConfiguration.cpp
        UserConfiguration.hpp
        WindowsFileHandler.cpp
        WindowsFileHandler.hpp)
