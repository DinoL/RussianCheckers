#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <QObject>
#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkConnection : public QObject
{
    Q_OBJECT

public:
    NetworkConnection();

    ~NetworkConnection()
    {}

    bool is_connected_remotely() const;
    bool is_listening() const;
    bool is_ready() const;
    static QHostAddress preferred_host_address
        (const QAbstractSocket::NetworkLayerProtocol protocol);
    void connect_remotely() {}
    void disconnect_remotely() {}
    void initialize() {}
    void quit() {}
    void set_listen();
    void stop_listening() {}

private:
    QPointer<QTcpSocket> _client_connection;
    QTcpServer _listening_socket;
    void prepare_connection_status() {}

private slots:
    void slot_accept_connection();
    void slot_client_connected() {}
    void slot_client_disconnected() {}
    void slot_disconnected(QAbstractSocket::SocketError error) {}
    void slot_update_board() {}

signals:
    void connected_to_client();
    void disconnected_from_client();
};

#endif // NETWORK_CONNECTION_H
