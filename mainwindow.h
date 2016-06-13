#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<iostream>
#include <QMainWindow>
#include<vector>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    vector<vector<pair<int,int>>> group_pixels(QImage& img);
    vector<pair<int,int>> git_boundaries(vector<pair<int,int>> group);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage thresholding(QImage image);
    QImage coloringgroup(QImage image,vector<pair<int,int>> group);
    bool IsRegularshape(vector<pair<int,int>> shape);
    pair<int,int> GetCenter(vector<pair<int,int>> shape);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
