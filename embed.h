// embed.h

#if !defined(_EMBED_H_)
#define _EMBED_H_ 1

#include <QEvent>
#include <QWindow>


class Embed : public QWindow {
	//	Q_OBJECT

public:
	explicit Embed();
	~Embed();

	void resize(const QSize &newSize);
	void begin();
	void end();

	void sendKeys(QString s);
	void sendKey(bool press, QKeyEvent *event);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	int childId;
};

#endif
