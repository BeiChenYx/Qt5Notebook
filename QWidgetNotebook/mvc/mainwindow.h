#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

/* MVC 架构思想在 Qt 中实现
 *
 * 1. 数据和窗口绑在一起的困境
 *    1> 需求: 需要将某个班级的学生以列表的形式展现出来，并且可以向班级中添加学生
 * 	  2> 首先创建两个类，StudentClass 和 Student
 *    3> 接下来，创建一个窗口用来展示学生姓名列表，并且可以录入学生学号和姓名，添加学生。该窗口类中保存了班级信息
 * 2. 现在需求改变了，客户提出要再创建一个表格来展示学生的姓名和所属班级(或者更复杂的页面更改)
 *    1> 为了解决这个问题，采用 MVC 的办法来分离数据和页面
 *    2> 在 MainWindows 中添加 model 成员变量 QStanddardItemModel 类作为视图的 model .
 * 	  3> 修改 MainWinws 构造函数
 *    4> 修改 按钮响应的槽
 * MVC的使用，使我们只需要关注更新model的部分，而不需要关心view该如何变化，因为当model改变了，
 * 所有关联它的view都会得到相应的更新，最令人兴奋的是你只需要维护一个model。
*/

class Student
{
public:
    QString getId() const{return id;}
    void setId(const QString &value){id = value;}
    QString getName() const{return name;}
    void setName(const QString &value){name = value;}

    bool isValid() const
    {
        return !id.isEmpty() && !name.isEmpty();
    }

private:
    QString id;
    QString name;
};


class StudentClass
{
public:
    QString getId() const{return id;}
    void setId(const QString &value){id = value;}
    QString getName() const{return name;}
    void setName(const QString &value){name=value;}

    bool add(const Student& student)
    {
        bool sucess = false;
        if(student.isValid())
        {
            if(!studentMap.contains(student.getId()))
            {
                studentMap.insert(student.getId(), student);
                sucess = true;
            }
        }
        return sucess;
    }

    QList<Student> getStudentList() const
    {
        return studentMap.values();
    }
private:
    QString id;
    QString name;
    QMap<QString, Student> studentMap; // key-学生
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    StudentClass studentclass; // 保留班级数据
    QStandardItemModel* model; // MVC 的方案
};

#endif // MAINWINDOW_H
