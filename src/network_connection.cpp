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

void NetworkConnection::set_listen()
{
    if(_listening_socket.isListening())
        return;

    _listening_socket.setMaxPendingConnections(1);

    QHostAddress address(QHostAddress::Any);
    quint16 port = 0; // set from UI

    _listening_socket.listen(address, port);

    prepare_connection_status();
}

void NetworkConnection::slot_accept_connection()
{
    auto socket = _listening_socket.nextPendingConnection();

    if(!socket)
        return;
    else if(_client_connection)
    {
        socket->abort();
        socket->deleteLater();
        return;
    }
    else
        _client_connection = socket;

    _client_connection->setParent(this);

    emit connected_to_client();
    connect(_client_connection,
            SIGNAL(disconnected()),
            this,
            SLOT(slot_client_disconnected()));
    connect(_client_connection,
            SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            _client_connection,
            SIGNAL(disconnected()));
    connect(_client_connection,
            SIGNAL(readyRead()),
            this,
            SLOT(slot_update_board()));
}
