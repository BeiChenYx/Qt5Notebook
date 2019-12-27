#include "threadsync.h"
#include <QDebug>

ThreadSync::ThreadSync(QObject *parent)
    : stopped(false)
{
}

void ThreadSync::run()
{
    // 线程的开启和停止
//    qreal i = 0;
//    while (!stopped) {
//        qDebug() << QString("In ThreadSync: %1.").arg(i++);
//        qDebug() << QString("ThreadId: ") << QThread::currentThreadId();
//        QThread::msleep(1000);
//    }

    // 线程之间的同
    while (!stopped) {
        int data = this->popFromTask();
        qDebug() << data;
        if(data > 0){
            emit handleResult(data * 100);
        }
        QThread::msleep(1000);
    }
}

void ThreadSync::stop()
{
    stopped = true;
}

void ThreadSync::pushToTask(int data)
{
    QMutexLocker locker(&mutex);
    this->msgQueue.enqueue(data);
}

int ThreadSync::popFromTask()
{
    QMutexLocker locker(&mutex);
    if(!this->msgQueue.isEmpty()){
        return this->msgQueue.dequeue();
    }else {
        return 0;
    }
}
