#include "bll.h"

Bll::Bll(mynamespace::NetWork *&pNetGetter) :
    m_pRecvMessageShunter (nullptr),
    m_pIdDistributor (nullptr),
    m_pCommandTransmitter (nullptr),
    m_pUserConfigUpdater(nullptr),
    m_pStatusPoller (nullptr),
    m_pDal (nullptr) ,
    m_pNetGetter(pNetGetter), 
    m_pReadyListLocker(nullptr) {
}

Bll::~Bll() {
    if (m_pRecvMessageShunter == nullptr) {
        delete m_pRecvMessageShunter;
        m_pRecvMessageShunter = nullptr;
    }
    if (m_pIdDistributor != nullptr) {
        delete m_pIdDistributor;
    }
    if (m_pReadyListLocker != nullptr) {
        delete m_pReadyListLocker;
        m_pReadyListLocker = nullptr;
    }
    if (m_pCommandTransmitter != nullptr) {
        delete m_pCommandTransmitter;
        m_pCommandTransmitter = nullptr;
    }
    if (m_pUserConfigUpdater != nullptr) {
        delete m_pUserConfigUpdater;
        m_pUserConfigUpdater = nullptr;
    }
    if (m_pStatusPoller != nullptr) {
        delete m_pStatusPoller;
        m_pStatusPoller = nullptr;
    }
    if (m_pDal != nullptr) {
        delete m_pDal;
        m_pDal = nullptr;
    }
}

bool Bll::init() {
    m_pDal = new Dal(m_pNetGetter); 
    m_pDal->init();
    m_pIdDistributor = new IdIdentifier(m_pDal);
    m_pCommandTransmitter = new TransitCenter(m_pDal); 
    m_pUserConfigUpdater = new UserConfigureUpdater(m_pDal);
    m_pStatusPoller = new PollingMachine(); 
    m_pRecvMessageShunter = new MessageShunter();
    m_pReadyListLocker = new Locker();

    m_pStatusPoller->setPollingObj(&m_readyDeviceList, m_pDal, m_pReadyListLocker);
    /*start polling*/
    pthread_t trd_polling = 0;
    pthread_create(&trd_polling, NULL, m_pStatusPoller->startPolling, m_pStatusPoller);
     
    return true;
}

int Bll::idenfity(int iTaskId, Message *pUndoTask) {
    m_pReadyListLocker->lock();
    if (!m_pIdDistributor->identityIdentification(pUndoTask, iTaskId, m_readyDeviceList, m_readyUserList)) {
        m_pReadyListLocker->unlock();
        return -1;
    }
    m_pReadyListLocker->unlock();
    return 0;
}
int Bll::transmit(int iTaskId, Message *pUndoTask, std::list <ReadyDevice> &readyDeviceList) {
    m_pReadyListLocker->lock();
    if (!m_pCommandTransmitter->forwardCommand(pUndoTask, iTaskId, readyDeviceList)) {
        m_pReadyListLocker->unlock();
        return -1;
    }
    m_pReadyListLocker->unlock();
    return 0;
}
int Bll::update(int iTaskId, Message *pUndoTask) {
    if (!m_pUserConfigUpdater->updateConfiguration(pUndoTask, iTaskId)) {
        return -1;
    }
    return 0;
}

bool Bll::pushMessage(int iTaskId, Message *pUndoTask) {
    int ret = m_pRecvMessageShunter->shuntMsg(pUndoTask);

    if (ret == -1) {
        return false; 
    }
    else if (ret == 0) {
        if (idenfity(iTaskId, pUndoTask) == -1) {
            return false;
        }
        m_pNetGetter->push_msg(iTaskId);
    }
    else if (ret == 1) {
        if (transmit(iTaskId, pUndoTask, m_readyDeviceList) == -1) {
            return false;
        }
    }
    else if (ret == 2) {
        if (update(iTaskId, pUndoTask) == -1) {
            return false;
        }
    }
    else if (ret == 3) {
        if (!m_pCommandTransmitter->getResponse(pUndoTask, iTaskId, m_readyUserList)) {
            return false;
        }
        return false; 
    }

    return true;
}

void Bll::addReadyUser(int iUserId, int iTaskId, unsigned char *pPeerSendAddr) {
    ReadyUser *pUser = new ReadyUser ();

    pUser->setUserId(iUserId);
    pUser->setTaskId(iTaskId);
    pUser->setPeerAddr(pPeerSendAddr);

    m_readyUserList.push_back(*pUser);
}

int Bll::getUserTask(int iUserId, unsigned char *&pPeerSendAddr) {/*return for taskId*/
    std::list <ReadyUser>::iterator i;
    for (i=m_readyUserList.begin(); i!=m_readyUserList.end(); ++i) {
        if ((*i).getUserId() == iUserId) {
            pPeerSendAddr = (*i).getPeerAddr();
            return (*i).getTaskId();
        }
    }
    return -1;
}
void Bll::removeReadyUser(int iUserId) {
	std::list <ReadyUser>::iterator i;
	for (i = m_readyUserList.begin(); i != m_readyUserList.end(); i++)
		if ((i->getUserId()) == iUserId) {
			std::cout << "UserId: " << i->getUserId() << std::endl << "TaskId: " << i->getTaskId() << std::endl <<"SendAddr: "<< i->getPeerAddr() << std::endl;
			m_readyUserList.erase(i);
			break;
		}
 }
void Bll::addReadyDevice(int iDeviceId, int iTaskId, unsigned char *pPeerSendAddr){ 
	ReadyDevice *pDevice = new ReadyDevice();

	pDevice->setDeviceId(iDeviceId);
	pDevice->setTaskId(iTaskId);
	pDevice->setPeerAddr(pPeerSendAddr);

	m_readyDeviceList.push_back(*pDevice);
}
int Bll::getDeviceTask(int iDeviceId, unsigned char *&pPeerSendAddr){ //return for taskId
	std::list <ReadyDevice>::iterator i;
	for (i = m_readyDeviceList.begin(); i != m_readyDeviceList.end(); ++i) {
		if ((*i).getDeviceId() == iDeviceId) {
			pPeerSendAddr = (*i).getPeerAddr();
			return (*i).getTaskId();
		}
	}
    return -1;
}
void Bll::removeReadyDevice(int iDeviceId){ 
	std::list <ReadyDevice>::iterator i;
	for (i = m_readyDeviceList.begin(); i != m_readyDeviceList.end(); i++)
		if ((i->getDeviceId()) == iDeviceId) {
			std::cout << "DeviceId: " << i->getDeviceId() << std::endl << "TaskId: " << i->getTaskId() << std::endl << "SendAddr: " << i->getPeerAddr() << std::endl;
			m_readyDeviceList.erase(i);
			break;
		}
}
void Bll::addTask(int iSourceId, int iDestinationId) {
	UndoTask *pTask = new UndoTask();

	pTask->setSourceId(iSourceId);
	pTask->setDestinationId(iDestinationId);

	m_undoTasks.push_back(*pTask);
}
UndoTask *Bll::popTask() {
	if (m_undoTasks.empty()) {
		return nullptr;
	}
	UndoTask *pTask = new UndoTask();
	memcpy(pTask, &m_undoTasks.front(), sizeof(UndoTask));
	m_undoTasks.pop_front();

	return pTask;
}


