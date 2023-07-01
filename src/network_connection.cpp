#include "network_connection.h"

NetworkConnection::NetworkConnection()
{}

bool NetworkConnection::is_connected_remotely() const
{
    return false;
}

bool NetworkConnection::is_listening() const
{
    return false;
}

bool NetworkConnection::is_ready() const
{
    return false;
}

QHostAddress NetworkConnection::preferred_host_address(const QAbstractSocket::NetworkLayerProtocol protocol)
{
    return QHostAddress();
}
