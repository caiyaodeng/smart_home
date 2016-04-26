/*************************************************************************
	> File Name: tcp_socket.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com 
	> Created Time: Fri 11 Mar 2016 09:00:57 AM EST
 ************************************************************************/

#include "tcp_socket.h"

namespace mynamespace {
    TcpSocket::TcpSocket()
    :
    m_socket (0),
    m_reference_count (1) ,
    m_send_buf (NULL),
    m_recv_buf (NULL) {
        m_send_buf = new Buffer ();
        m_recv_buf = new Buffer ();
    }
    TcpSocket::~TcpSocket() {
        if (m_send_buf != NULL) {
            delete m_send_buf;
            m_send_buf = NULL;
        }
        if (m_recv_buf != NULL) {
            delete m_recv_buf;
            m_recv_buf = NULL;
        }
    }

    int TcpSocket::send_buf() {
        uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);
        m_lock.lock();

        if (m_reference_count > 0) {
            m_send_pos += m_send_buf->get_size();
            m_reference_count --;
        }
        int left = m_send_pos;
        int ret = m_send_buf->get_buf(buf);
        int send_pos = 0;
        std::cout << "left:" << left << std::endl;
        m_lock.unlock();
        //std::cout << "TcpSocket :senf_buf() ret:" << ret << std::endl;
        //std::cout << "TcpSocket :senf_buf() fd:" << m_socket << std::endl;
        //std::cout << "TcpSocket :senf_buf() left:" << left << std::endl;
        if (ret == 0) {
            return 0;
        }

        if (left == 0) {
            return 0;
        }

        std::cout << "send:" << buf << std::endl;
        
        while ((ret = write(m_socket, buf+send_pos, left)) > 0) {
            send_pos += ret;
            left -= ret;
        }

        if (left == 0) {
            //m_send_pos = 0;
            std::cout << "send_ok:" << buf << std::endl;
            return 0;
        }

        if (ret < 0 && (errno ==EAGAIN || errno == EWOULDBLOCK)) {
            m_send_pos -= ret;
           return -1; 
        }
               
        return -1;
    }
    int TcpSocket::recv_buf() {
        int ret = 0;
        uint8_t recv_buf[BUFFER_SIZE];
        memset(recv_buf, 0, BUFFER_SIZE);
        m_send_pos = 0;
        
        while ((ret = read (m_socket, recv_buf, BUFFER_SIZE)) != 0) {
            if (ret <= BUFFER_SIZE) {
                break;
            }
        }
        if (ret == 0) {
            std::cout << "peer closed !" << std::endl; 
            return -1; 
        }

        m_lock.lock();
        m_recv_buf->put_buf(recv_buf, ret);
        std::cout << "recv num:" << m_recv_buf->get_buf((uint8_t *)recv_buf);
        m_lock.unlock();
        std::cout << "recv:" << recv_buf << std::endl;
        return 0;
    }
    bool TcpSocket::reset() {
        m_socket = 0;
        m_lock.lock(); 
        m_send_buf->clean();
        m_recv_buf->clean();
        m_lock.unlock();
        return true;}

    int TcpSocket::get_msg(uint8_t *buf_out) {
        return (m_recv_buf->get_buf(buf_out));
    }

    bool TcpSocket::put_msg(const uint8_t *buf_in, const int buf_length_in) {
       return (0 != m_send_buf->put_buf(buf_in, buf_length_in)); 
    }
    void TcpSocket::set_fd(int conn_fd) {
        m_socket = conn_fd; 
    }

    void TcpSocket::close() {
        ::close(m_socket);
    }

    void TcpSocket::process() {
        std::cout << "begin_________" << std::endl;
        uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);
        m_lock.lock();
        m_lock.unlock();
        int ret = m_recv_buf->get_buf(buf);
        if (ret == 0) {
            ERR_EXIT("TcpSocket", "process");
        }
        std::cout <<"cpy:"<< buf;
        m_lock.lock();
        m_send_buf->put_buf(buf, ret);
        m_lock.unlock();
        this->send_buf();
        //fflush(m_socket)
        free(buf);
        buf = NULL;
        std::cout << "end_______________" << std::endl;
    }
}

