#include "network_connection.h"

#include <QNetworkInterface>

NetworkConnection::NetworkConnection()
{
    connect(&_listening_socket,
            SIGNAL(newConnection()),
            this,
            SLOT(slot_accept_connection()));
}

bool NetworkConnection::is_connected_remotely() const
{
    return _client_connection &&
           _client_connection->state() == QAbstractSocket::ConnectedState;
}

bool NetworkConnection::is_listening() const
{
    return _listening_socket.isListening();
}

bool NetworkConnection::is_ready() const
{
    return is_connected_remotely();
}

QHostAddress NetworkConnection::preferred_host_address(
    const QAbstractSocket::NetworkLayerProtocol protocol)
{
    for (const auto& interface : QNetworkInterface::allInterfaces())
    {
        const auto flags = interface.flags();
        if(!(flags & QNetworkInterface::IsLoopBack)
            && (flags & QNetworkInterface::IsUp))
        {
            for (const auto& address : interface.addressEntries())
            {
                if(address.ip().protocol() == protocol)
                    return address.ip();
            }
        }
    }

    if(protocol == QAbstractSocket::IPv4Protocol)
        return QHostAddress(QHostAddress::LocalHost);
    else
        return QHostAddress(QHostAddress::LocalHostIPv6);
}
