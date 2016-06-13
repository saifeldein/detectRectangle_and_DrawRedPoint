#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<vector>
#include<math.h>
#include "QDebug"
#include "QFileDialog"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   QString QName = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:\\tmp projects and folders\\images");
   QPixmap pix(QName);
   ui->label->setPixmap(pix);
   QImage image = pix.toImage();
//   image.scaled(200,200,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    image=image.scaled((2448/8),(3264/8));

  QImage im;
  im=thresholding(image);

  QPixmap newpix=pix.fromImage(im,Qt::AutoColor);
  newpix = newpix.scaled((2448/8),(3264/8));
 ui->label->setPixmap(newpix);
  vector<vector<pair<int,int>>> groups=group_pixels(im);
  for(int i=0;i<groups.size();i++)
  {
        //im=coloringgroup(im,groups[i]);
        vector<pair<int,int>> tmp=git_boundaries(groups[i]);

        if(IsRegularshape(tmp))
         {

              pair<int,int> center=GetCenter(tmp);
              for(int iter1=center.first-2;iter1<=center.first+2;iter1++)
              {
                  for(int iter2=center.second-2;iter2<=center.second+2;iter2++)
                  {

                         image.setPixel(iter1,iter2,qRgb(255,0,0));
                  }
              }
          }

  }
      newpix=newpix.fromImage(image,Qt::AutoColor);
      ui->label->setPixmap(newpix);
}
QImage MainWindow::thresholding(QImage image)
{
    QImage im=image;
    QRgb pix;
    QColor whitecolor;
    whitecolor.setGreen(255);
    whitecolor.setRed(255);
    whitecolor.setBlue(255);
    for(int i=0;i<image.width();i++)
    {
        for(int j=0;j<image.height();j++)
        {
            pix=image.pixel(i,j);

            int g = qGray(pix);
            if(g>190)
                im.setPixel(i,j,qRgb(255,255,255));
            else
                im.setPixel(i,j,qRgb(0,0,0));
        }
    }

    return im;
}


QImage MainWindow::coloringgroup(QImage image,vector<pair<int,int>> group)
{
    QImage im=image;
    QRgb pix;
    QColor whitecolor;
    whitecolor.setGreen(255);
    whitecolor.setRed(255);
    whitecolor.setBlue(255);
    for(int i=0;i<group.size();i++)
    {
      im.setPixel(group[i].first,group[i].second,qRgb(255,0,0));
    }
    return im;
}


vector<vector<pair<int,int>>> MainWindow::group_pixels(QImage& img)
{
    QColor whitecolor;
    whitecolor.setGreen(255);
    whitecolor.setRed(255);
    whitecolor.setBlue(255);
    vector<vector<pair<int,int>>> groups;
      vector<vector<int>> visited(img.width());
      for(int i=0;i<img.width();i++)
      {   visited[i].resize(img.height());
          for(int j=0;j<img.height();j++)
            visited[i][j]=0;
      }
    for(int i=0;i<img.width();i++)
     {
        for(int j=0;j<img.height();j++)
       {
            if(visited[i][j]!=0)
              continue;
         vector<pair<int,int>> openlist;

         vector<pair<int,int>> tmpgroup;

         pair<int,int> curr;
         curr.first=i;
         curr.second =j;
         if(qGray(img.pixel(curr.first,curr.second))!=255)
             continue;
         openlist.push_back(curr);
         while(!openlist.empty())
         {
             curr=openlist.back();
             openlist.pop_back();
             tmpgroup.push_back(curr);

             if(curr.first+1<img.width()&&qGray(img.pixel(curr.first+1,curr.second))==255&&visited[curr.first+1][curr.second]==0)
              {
                 visited[curr.first+1][curr.second]= 1;
                  pair<int,int> tmp;
                  tmp.first=curr.first+1;
                  tmp.second=curr.second;
                  openlist.push_back(tmp);
             }
             if(curr.first-1>=0&&qGray(img.pixel(curr.first-1,curr.second))==255&&visited[curr.first-1][curr.second]==0)
              {
                 visited[curr.first-1][curr.second]= 1;
                  pair<int,int> tmp;
                  tmp.first=curr.first-1;
                  tmp.second=curr.second;
                  openlist.push_back(tmp);
             }
             if(curr.second+1<img.height()&&qGray(img.pixel(curr.first,curr.second+1))==255&&visited[curr.first][curr.second+1]==0)
              {
                 visited[curr.first][curr.second+1]= 1;
                  pair<int,int> tmp;
                  tmp.first=curr.first;
                  tmp.second=curr.second+1;
                  openlist.push_back(tmp);
             }
             if(curr.second-1>=0&&qGray(img.pixel(curr.first,curr.second-1))==255&&visited[curr.first][curr.second-1]==0)
              {
                 visited[curr.first][curr.second-1]= 1;
                  pair<int,int> tmp;
                  tmp.first=curr.first;
                  tmp.second=curr.second-1;
                  openlist.push_back(tmp);
             }



         }

         groups.push_back(tmpgroup);


         }
    }

    return groups;
}




vector<pair<int,int>> MainWindow::git_boundaries(vector<pair<int,int>> group)
{
    int maxX=INT_MIN,maxY=INT_MIN,minX=INT_MAX,minY=INT_MAX;

    for(int i=0;i<group.size();i++)
    {
        if(group[i].first>maxX)
            maxX=group[i].first;
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].first==maxX)
        {
            if(group[i].second>maxY)
                maxY=group[i].second;
        }
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].first<minX)
            minX=group[i].first;
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].first==minX)
        {
            if(group[i].second<minY)
                minY=group[i].second;
        }
    }
    vector<pair<int,int>> boundaries(4);
    boundaries[0].first=minX;
    boundaries[0].second=minY;
    boundaries[3].first=maxX;
    boundaries[3].second=maxY;
    maxX=INT_MIN;
    maxY=INT_MIN;
    minX=INT_MAX;
    minY=INT_MAX;

    for(int i=0;i<group.size();i++)
    {
        if(group[i].second>maxY)
            maxY=group[i].second;
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].second==maxY)
        {
            if(group[i].first>maxX)
                maxX=group[i].first;
        }
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].second<minY)
            minY=group[i].second;
    }
    for(int i=0;i<group.size();i++)
    {
        if(group[i].second==minY)
        {
            if(group[i].first<minX)
                minX=group[i].first;
        }
    }
    boundaries[1].first=minX;
    boundaries[1].second=minY;
    boundaries[2].first=maxX;
    boundaries[2].second=maxY;
    return boundaries;
}

bool MainWindow::IsRegularshape(vector<pair<int,int>> shape)
{

    if(shape[3].first-shape[0].first<15)
        return false;
    if(shape[2].second-shape[1].second<15)
        return false;

    double diameter1=sqrt(pow((double)(shape[3].first-shape[0].first),2)+pow((double)(shape[3].second-shape[0].second),2));
    double diameter2=sqrt(pow((double)(shape[2].first-shape[1].first),2)+pow((double)(shape[2].second-shape[1].second),2));
    double diameter3=sqrt(pow((double)(shape[3].first-shape[2].first),2)+pow((double)(shape[3].second-shape[2].second),2));
    double diameter4=sqrt(pow((double)(shape[0].first-shape[1].first),2)+pow((double)(shape[0].second-shape[1].second),2));
    if(abs((diameter1/diameter2)-1)<=0.2&&(abs((diameter3/diameter4)-1)<=0.2||(diameter3==0&&diameter4==0)))
        return true;

    return false;

}
pair<int,int> MainWindow::GetCenter(vector<pair<int,int>> shape)
{
 int centerX,centerY;

   centerX=(shape[3].first+shape[0].first)/2;
   centerY=(shape[2].second+shape[1].second)/2;
   pair<int,int> center;
   center.first=centerX;
   center.second=centerY;
   cout<<shape[3].first<<"  "<<shape[0].first<<"  "<<shape[2].second<<"  "<<shape[1].second<<"  "<<center.first<<"  "<<center.second;
   cout<<endl;
   return center;
}


