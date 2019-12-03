#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_botaoCalculos_clicked()
{
    //Abre o log para leitura
    QString file_name = QFileDialog::getOpenFileName(
                Q_NULLPTR,"Selecione um arquivo","C://");
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());

    //Identifica o conteúdo log
    QTextStream in(&file);

    //Lê e salva a primeira linha do log na varíavel coordenadas
    QString coordenadas = in.readLine();

    int qtd_linhas = 0;

    //Variáveis para latitude
    double soma_latitude = 0;
    double latitude_minima = coordenadas.split(" ")[0].toDouble();
    double latitude_maxima = coordenadas.split(" ")[0].toDouble();

    //Variáveis para longitude
    double soma_longitude = 0;
    double longitude_minima = coordenadas.split(" ")[1].toDouble();
    double longitude_maxima = coordenadas.split(" ")[1].toDouble();

    //Variáveis para altitude
    double soma_altitude = 0;
    double altitude_minima = coordenadas.split(" ")[2].toDouble();
    double altitude_maxima = coordenadas.split(" ")[2].toDouble();

    //Lê e salva a linha atual do log na variável coordenadas a cada iteração
    //Para na última linha
    while (!in.atEnd()) {
            QString coordenadas = in.readLine();

            //Calcula latitude máxima e mínima
            double latitude = coordenadas.split(" ")[0].toDouble();
            soma_latitude+=latitude;
            if (latitude>latitude_maxima){
                latitude_maxima=latitude;
            }
            if (latitude<latitude_minima){
                latitude_minima=latitude;
            }

            //Calcula longitude máxima e mínima
            double longitude = coordenadas.split(" ")[1].toDouble();
            soma_longitude+=longitude;
            if (longitude>longitude_maxima){
                longitude_maxima=longitude;
            }
            if (longitude<longitude_minima){
                longitude_minima=longitude;
            }

            //Calcula altitude máxima e mínima
            double altitude = coordenadas.split(" ")[2].toDouble();
            soma_altitude+=altitude;
            if (altitude>altitude_maxima){
                altitude_maxima=altitude;
            }
            if (altitude<altitude_minima){
                altitude_minima=altitude;
            }

            //Conta linhas para calcular média
            qtd_linhas++;
    }

    //Calcula latitude média
    double media_latitude = soma_latitude/qtd_linhas;

    //Calcula longitude média
    double media_longitude = soma_longitude/qtd_linhas;

    //Calcula altitude média
    double media_altitude = soma_altitude/qtd_linhas;

    //Exibe resultados para o usuário
    ui->textCalculos->setText(
                "LATITUDE:\nMédia: "+QString::number(media_latitude)+
                "\nMáxima: "+QString::number(latitude_maxima)+
                "\nMínima: "+QString::number(latitude_minima)+

                "\n\nLONGITUDE:\nMédia: "+QString::number(media_longitude)+
                "\nMáxima: "+QString::number(longitude_maxima)+
                "\nMínima: "+QString::number(longitude_minima)+

                "\n\nALTITUDE:\nMédia: "+QString::number(media_altitude)+
                "\nMáxima: "+QString::number(altitude_maxima)+
                "\nMínima: "+QString::number(altitude_minima)
                );
}
