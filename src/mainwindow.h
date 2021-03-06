#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef QT_NO_KEYWORDS
#define QT_NO_KEYWORDS
#endif

#include <QMainWindow>
#include <QtDBus/QDBusInterface>
#include "configuration.h"
#include "bioAuthentication/biodeviceview.h"

namespace Ui {
class MainWindow;
}

enum ScreenState {
	UNDEFINED,
	LOCKSCREEN,
	XSCREENSAVER,
	XSCREENSAVER_BY_IDLE /* Xscreensaver is activated by session idle */
};

enum ProgramState {
	IDLE,
	SHOW_PAM_MESSAGE,
	GET_PASSWORD,
	WAIT_AUTH_STATUS,
	AUTH_FAILED
};

/* https://www.narf.ssji.net/~shtrom/wiki/projets/gnomescreensavernosession */
enum SessionStatus {
	SESSION_AVAILABLE = 0,
	SESSION_INVISIBLE = 1,
	SESSION_BUSY = 2,
	SESSION_IDLE = 3
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
    void setShowSaver(bool showSaver);
    void showDialog();

private:
	void setRealTimeMouseTracking();
	void embedXScreensaver();
	void handleKeyPressEvent(QKeyEvent *event);
	void handleMouseMoveEvent(QMouseEvent *event);
	void switchToLockscreen();
	void switchToXScreensaver();
	void startXScreensaverWithoutAuth();
	void constructUI();
	void uiGetReady(bool ready);
	void setWindowStyle();
	void lockscreenFollowCursor(QPoint cursorPosition);
	QString getUserAvatarPath(QString username);
	bool signalSenderFilter(int signalSenderPID);
    void setPasswordVisible(bool visible);
    void setCapsLockWarn();
    void clearSavers();

protected:
	void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
    void closed();

public Q_SLOTS:
	void FSMTransition(int signalSenderPID); /* Transition FSM states according to signal */

private Q_SLOTS:
	void onUnlockClicked();
	void onPasswordEnter();
    void onSessionIdle();
public Q_SLOTS:
    void onGlobalKeyPress(int keyId);
    void onGlobalMouseMove(int x, int y);

private:
	Ui::MainWindow *ui;
	unsigned long int winId;
	enum ScreenState screenState;
	QList<int> xscreensaverPIDList;
	QList<QWidget *> widgetXScreensaverList;
	enum ProgramState programState;
	int toAuthChild[2];
	int toParent[2];
	int authPID;
	Configuration *configuration;
	QPixmap pixmap;
	QDBusInterface *interface;
    BioDeviceView *widgetBioDevices;
    bool isActivated;
    bool isPasswdFailed;
    QTimer *timer;
    bool showSaver;
};

#endif // MAINWINDOW_H
