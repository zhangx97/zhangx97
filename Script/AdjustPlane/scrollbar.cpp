/*
换成QML，这个界面目前没用了


*/



#include "scrollbar.h"
#include "QString"

ScrollBar::ScrollBar(QWidget *parent, ScrollType nType):QWidget (parent),
    m_nType(nType),
    m_nCurrentValue(0),
    m_nOffSet(0),
    m_nMax(0),
    m_nMin(0),
    m_nMousePos(0),
    m_nDevice(5),
    m_nStep(0.1)
{
    this->setFixedSize(parent->size());
}

ScrollBar::~ScrollBar()
{

}

void ScrollBar::setColor(QColor &pColor, ColorType nType)
{
    switch (nType)
    {
        case 1:
        {
            m_cBackground = pColor;
            break;
        }
        case 2:
        {
            m_cLine = pColor;
            break;
        }
        case 3:
        {
            m_cCurrentText = pColor;
            break;
        }
        case 4:
        {
            m_cDisableText = pColor;
            break;
        }
        default:
            break;
    }
}

void ScrollBar::mousePressEvent(QMouseEvent *event)
{
    m_nMousePos = m_nType == ScrollType::HORIZONTAL ? event->pos().x() : event->pos().y();
    update();
}

void ScrollBar::mouseMoveEvent(QMouseEvent *event)
{
    int nMousePos = m_nType == ScrollType::HORIZONTAL?event->pos().x(): event->pos().y();

    if(m_nCurrentValue == m_nMin && nMousePos >= m_nMousePos ||
           m_nCurrentValue == m_nMax && nMousePos <= m_nMousePos)
    {
        return;
    }
    int nTarget = m_nType == ScrollType::HORIZONTAL ? this->width():this->height() ;
    int nOffSet = nMousePos - m_nMousePos;

    //判断鼠标移动的距离是否大于最小偏移量 如果大于偏移量 则将偏移量重置为最小偏移量 目的是避免界面出现跨越显示
    if(nOffSet > (nTarget / m_nDevice))	//(nTarget / m_nDevice) 为一次偏移的最小值 也就是一个字体的显示的大小边界值
    {
        nOffSet = nTarget / m_nDevice;
    }
    else if(nOffSet < -nTarget / m_nDevice)
    {
        nOffSet = -nTarget / m_nDevice;
    }

    //nOffSet的正负代表偏移的方向
    m_nOffSet = nOffSet;
    update();
}

void ScrollBar::mouseReleaseEvent(QMouseEvent *event)
{
    int nTarget = m_nType == ScrollType::HORIZONTAL ? this->width():this->height() ;
    int nOffSet = m_nOffSet;
    //单个数字大小的一半，当移动距离超过一半的时候，说明该换下个数了
    int nJudge = nOffSet < 0 ? -(nTarget / (m_nDevice * 2)) : nTarget / (m_nDevice * 2);
    if(nOffSet < 0)
    {
        if(nOffSet < nJudge)
        {
            m_nOffSet = 0;
            goto UPDATE;
        }
        m_nOffSet = -nTarget / m_nDevice;
        goto UPDATE;
    }
    if (nOffSet < nJudge)
    {
        m_nOffSet = 0;
        goto UPDATE;
    }
    m_nOffSet = nTarget / m_nDevice;

UPDATE:
    update();
}

void ScrollBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    if(m_nMin == m_nMax)
    {
        return;
    }

    int nTarget = m_nType == ScrollType::HORIZONTAL ? this->width():this->height();
    int nOffSet = m_nOffSet;
    if(nOffSet >= (nTarget / m_nDevice) && m_nCurrentValue > m_nMin)
    {
        m_nMousePos += nTarget / m_nDevice;
        nOffSet -= nTarget / m_nDevice;
        this->setValue(m_nCurrentValue - m_nStep);
        goto PAINTE;
    }
    else if(nOffSet <= -nTarget / m_nDevice && m_nCurrentValue < m_nMax)
    {
        m_nMousePos -= nTarget / m_nDevice;
        nOffSet += nTarget / m_nDevice;
        this->setValue(m_nCurrentValue + m_nStep);
    }

//绘制界面
PAINTE:
    if(getValue() == m_nMax || getValue() == m_nMin)
    {
        nOffSet = 0;
    }
    m_nOffSet = nOffSet;

//	首先绘制背景
    paintBackground(&painter);
//	绘制线条
    paintLine(&painter);
    int nFontSize = 14;
//	绘制当前字体
    paintText(&painter, m_nCurrentValue, nOffSet, nFontSize);
//	绘制两边的字体
    for (int nIndex = 1; nIndex <= m_nDevice / 2; ++nIndex)
    {
        nFontSize -= 2;
        if (m_nCurrentValue - m_nStep * nIndex >= m_nMin)
        {
//			两边字体的偏移量是通过距离计算的
            paintText(&painter, m_nCurrentValue - m_nStep * nIndex, nOffSet - nTarget / m_nDevice * nIndex, nFontSize);
        }

        if (m_nCurrentValue + m_nStep * nIndex <= m_nMax)
        {
            paintText(&painter, m_nCurrentValue + m_nStep * nIndex, nOffSet + nTarget / m_nDevice * nIndex, nFontSize);
        }
    }
//  将父窗口发送目的是能够准确的找到是属于哪个父类(假设界面有多个)
    emit signal_currentValueChange(getValue(), this->parentWidget());
}

void ScrollBar::paintBackground(QPainter *pPainter)
{
    pPainter->save();
    pPainter->setPen(Qt::NoPen);
    pPainter->setBrush(m_cBackground);
    pPainter->drawRect(rect());
    pPainter->restore();
}

void ScrollBar::paintLine(QPainter *pPainter)
{
    int nWidth = this->width();
    int nHeight = this->height();

    pPainter->save();
    pPainter->setBrush(Qt::NoBrush);

    QPen pen = pPainter->pen();
    pen.setWidth(1);
    pen.setColor(m_cLine);
    pen.setCapStyle(Qt::RoundCap);
    pPainter->setPen(pen);

//	绘制线条需要指定线条的起始坐标, 对于不同类型的滚动屏，坐标也有不同的数值

    for(int nIndex = 2; nIndex <= 3; nIndex++)
    {
//		对于垂直滚动屏来说，线条的Y值是不变的，同理对于水平的滚动屏来说，线条的X值是不变的
        int nPosX = m_nType == ScrollType::HORIZONTAL ? nWidth / 5 * nIndex : 0 ;
        int nPosY = m_nType == ScrollType::HORIZONTAL ? 0 : nHeight / 5 * nIndex ;
        int nEndPosX = m_nType == ScrollType::HORIZONTAL ? nPosX : nHeight;
        int nEndPosY = m_nType == ScrollType::HORIZONTAL ? nHeight : nPosY;

        pPainter->drawLine(nPosX, nPosY, nEndPosX,  nEndPosY);
    }
    pPainter->restore();
}

void ScrollBar::paintText(QPainter *pPainter, float nValue, float nOffSet, int nFontSize)
{
    pPainter->save();

    int nWidth = this->width();
    int nHeight = this->height();
//	下面注释掉的两行是通过整个界面的长(高)来控制字体的大小
//    int nTarget = m_nType == ScrollType::VERTICAL ? this->height() : this->width();
//    font.setPixelSize((nTarget - qAbs(nOffSet)) / m_nDevice);
    QFont font = QFont("Helvetica", 5);
    font.setPixelSize(nFontSize);
    QColor nColor = nOffSet == 0 ? m_cCurrentText : m_cDisableText;
    QPen pen = pPainter->pen();
    pen.setColor(nColor);
    pPainter->setPen(pen);
    pPainter->setBrush(Qt::NoBrush);
    pPainter->setFont(font);

    int textHeight = pPainter->fontMetrics().height();
    int initY = nHeight / 2 + nOffSet - textHeight / 2;

    if(m_nType == ScrollType::HORIZONTAL)
    {
        int textWidth = pPainter->fontMetrics().width(QString::number(nValue,'f',2));
        int initX = nWidth / 2 + nOffSet - textWidth / 2;
        pPainter->drawText(QRect(initX, 0, 20, nHeight), Qt::AlignCenter, QString::number(nValue));
//        pPainter->drawText(QRect(initX, 0, textWidth, nHeight), Qt::AlignCenter, QString::number(nValue));
//      有个小问题，当这儿使用下面的语句时，左边的0和1就会不显示，暂时未纠结其具体原因
        goto NEXT;
    }

    pPainter->drawText(QRect(0, initY, nWidth, textHeight), Qt::AlignCenter, QString::number(nValue));

NEXT:
    pPainter->restore();
}



