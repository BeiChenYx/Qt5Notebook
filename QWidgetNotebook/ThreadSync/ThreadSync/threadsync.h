#ifndef THREADSYNC_H
#define THREADSYNC_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutexLocker>
#include <QMutex>


/*
 * 线程的创建停止和线程之间的同步
 */
class ThreadSync : public QThread
{
    Q_OBJECT
public:
    explicit ThreadSync(QObject *parent = nullptr);
    void stop();

    void pushToTask(int data);
    int popFromTask();

protected:
    void run();

signals:
    void handleResult(int result);

// 线程没有调用 exec() 方法之前是无法接收槽
public slots:

private:
    volatile bool stopped;

    // 线程通信的的队列
    QQueue<int>   msgQueue;
    // 通信的互斥锁
    QMutex        mutex;
};

#endif // THREADSYNC_H
