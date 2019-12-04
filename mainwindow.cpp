#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <qmath.h>
#include <QtMath>

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

void MainWindow::on_botaoConversao_clicked()
{
    //Abre o log para leitura
    QString file_name = QFileDialog::getOpenFileName(
                Q_NULLPTR,"Selecione um arquivo","C://");
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());

    //Identificação para ler o conteúdo log original
    QTextStream in(&file);

    //Cria novo arquivo
    QString new_file_name = QFileDialog::getSaveFileName(
                Q_NULLPTR,"Selecione uma pasta para salvar","/Desktop/conversao.txt");
    QFile new_file(new_file_name);
    new_file.open(QIODevice::WriteOnly | QIODevice::Text);

    //Identificação para escrever no log convertido
    QTextStream out(&new_file);

    //Loop para leitura e conversão de cada linha do log
    while (!in.atEnd()) {
        //Lê e salva a primeira linha do log na varíavel coordenadas
        QString coordenadas = in.readLine();

        //Converte para double as strings que representam latitude e longitude respectivamente
        double latitude = coordenadas.split(" ")[0].toDouble();
        double longitude = coordenadas.split(" ")[1].toDouble();

        //Converte latitude para graus, minutos e segundos
        int lat_degree = (int)latitude;
        int lat_minutes = (int) ( (latitude - (double)lat_degree) * 60.0);
        double lat_seconds = (double) ( (latitude - (double)lat_degree - (double)lat_minutes / 60.0) * 60.0 * 60.0 );

        //Converte longitude para graus, minutos e segundos
        int long_degree = (int)longitude;
        int long_minutes = (int) ( (longitude - (double)long_degree) * 60.0);
        double long_seconds = (double) ( (longitude - (double)long_degree - (double)long_minutes / 60.0) * 60.0 * 60.0 );

        //Escreve uma nova linha com a conversão no novo arquivo criado
        out << QString::number(lat_degree)+"° "+
               QString::number(lat_minutes)+"' "+
               QString::number(lat_seconds, 'f', 4)+"''\t"+
               QString::number(long_degree)+"° "+
               QString::number(long_minutes)+"' "+
               QString::number(long_seconds, 'f', 4)+"''\n";
    }
    new_file.close();
}
