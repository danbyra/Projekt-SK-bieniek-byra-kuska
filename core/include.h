#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <random>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>

#include <QTimer>
#include <QElapsedTimer>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

// main to apka a debug to testy
#define MAIN // MAIN / DEBUG

// stary sygnal do checkboxa zostal usuniety w 6.9 a nowy zostal dodany w 6.7
#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
#define STARY_CHECKBOX
#endif
