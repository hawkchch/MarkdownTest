#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QTextTable>
#include <QTextFrame>
#include <QTextTableFormat>
#include <QDebug>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>


#include <memory>
#include "maddy/parser.h"

//void setSingleLineTable(QTextBrowser *edit)
//{
//    QTextDocument *doc = edit->document();
//    QTextFrame::Iterator it = doc->rootFrame()->begin();
//    while (!it.atEnd()) {
//        QTextFrame *frame = it.currentFrame();
//        if (frame && frame->format().intProperty(QTextFormat::UserProperty) == 0) {
//            if (QTextTable *tbl = qobject_cast<QTextTable *>(frame)) {
//                QTextTableFormat fmt = tbl->format();
//                fmt.setCellSpacing(-1);
//                fmt.setCellPadding(5);
//                fmt.setBorder(1);
//                tbl->setFormat(fmt);
//            }
//        }
//        ++it;
//    }
//}

bool scrollToHeading(QTextBrowser *browser, const QString &headingText) {
    QTextDocument *doc = browser->document();
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        QString text = block.text();
        if (text == headingText) {
            QTextCharFormat fmt = block.charFormat();
            if (fmt.font().bold()) {
                QRectF rect = doc->documentLayout()->blockBoundingRect(block);
                QScrollBar *vScrollBar = browser->verticalScrollBar();
                vScrollBar->setValue(rect.y());
                return true;
            }
        }
        block = block.next();
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenLinks(false);
    ui->textBrowser->setOpenExternalLinks(false);

    connect(ui->textEdit, &QTextEdit::textChanged, ui->textBrowser, [this](){
        QString md = ui->textEdit->toPlainText();
        std::stringstream markdownInput(md.toStdString());
        std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
        std::string htmlOutput = parser->Parse(markdownInput);
        QString html = QString::fromStdString(htmlOutput);
        QString darkTheme = QString::fromUtf8(R"EOF(
        body{
            font-family: "SF Pro Text","Helvetica Neue",Arial,"Microsoft YaHei",sans-serif;
            font-size: 15px;
            line-height: 1.6;
            color: #d4d4d4;
            background: #1e1e1e;
            margin: 20px;
            padding: 0;
        }
        h1,h2,h3,h4,h5,h6{color:#569cd6;font-weight:600;margin:24px 0 10px;}
        h1{border-bottom:1px solid #3e3e3e;font-size:2em;}
        h2{font-size:1.5em;}
        h3{font-size:1.25em;}
        a{color:#3794ff;text-decoration:none;}
        a:hover{text-decoration:underline;}
        code{
            background:#313131;
            color:#d7ba7d;
            padding:2px 4px;
            border-radius:3px;
            font-family:"Consolas","Monaco","Courier New",monospace;
            font-size:0.9em;
        }
        pre{
            background:#1d1d1d;
            border:1px solid #3e3e3e;
            border-radius:5px;
            padding:12px;
            overflow-x:auto;
            line-height:1.45;
        }
        pre code{background:none;padding:0;color:#d4d4d4;}
        blockquote{
            margin:0 0 0 10px;
            padding-left:10px;
            border-left:3px solid #6a9955;
            color:#9cdcfe;
        }
        hr{border:none;border-top:1px solid #3e3e3e;margin:24px 0;}
        table{border-collapse:collapse;margin:1em 0;}
        th,td{padding:6px 12px;border:1px solid #3e3e3e;}
        th{background:#313131;color:#569cd6;}
        tr:nth-child(even){background:#252525;}
        img{max-width:100%;height:auto;border-radius:4px;}
        ul.task-list{list-style:none;padding-left:0;}
        input[type=checkbox]{margin-right:6px;accent-color:#569cd6;}
        )EOF");
        QString lightTheme = QString::fromUtf8(R"EOF(
                                              /* VSCode Light Theme 基础样式 */
                                                 body {
                                                     font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
                                                     font-size: 14px;
                                                     line-height: 1.6;
                                                     color: #333333;
                                                     background-color: #ffffff;
                                                     margin: 0;
                                                     padding: 20px 40px;
                                                     max-width: 900px;
                                                     margin: 0 auto;
                                                 }
                                                 /* 标题样式 */
                                                 h1 {
                                                     color: #000000;
                                                     font-size: 2.2em;
                                                     font-weight: 600;
                                                     border-bottom: 1px solid #e1e4e8;
                                                     padding-bottom: 0.3em;
                                                     margin-top: 1.5em;
                                                     margin-bottom: 0.8em;
                                                 }
                                                 h2 {
                                                     color: #000000;
                                                     font-size: 1.8em;
                                                     font-weight: 600;
                                                     border-bottom: 1px solid #e1e4e8;
                                                     padding-bottom: 0.3em;
                                                     margin-top: 1.5em;
                                                     margin-bottom: 0.8em;
                                                 }
                                                 h3 {
                                                     color: #000000;
                                                     font-size: 1.4em;
                                                     font-weight: 600;
                                                     margin-top: 1.2em;
                                                     margin-bottom: 0.6em;
                                                 }
                                                 h4 {
                                                     color: #000000;
                                                     font-size: 1.2em;
                                                     font-weight: 600;
                                                     margin-top: 1em;
                                                     margin-bottom: 0.5em;
                                                 }
                                                 h5, h6 {
                                                     color: #000000;
                                                     font-size: 1.1em;
                                                     font-weight: 600;
                                                     margin-top: 0.8em;
                                                     margin-bottom: 0.4em;
                                                 }
                                                 /* 段落和文本 */
                                                 p {
                                                     margin: 1em 0;
                                                     color: #333333;
                                                 }
                                                 /* 链接样式 */
                                                 a {
                                                     color: #0366d6;
                                                     text-decoration: none;
                                                 }
                                                 a:hover {
                                                     color: #0366d6;
                                                     text-decoration: underline;
                                                 }
                                                 /* 代码样式 */
                                                 code {
                                                     background-color: #f6f8fa;
                                                     border-radius: 3px;
                                                     padding: 0.2em 0.4em;
                                                     font-family: 'Cascadia Code', 'Consolas', 'Monaco', monospace;
                                                     font-size: 0.9em;
                                                     color: #e01e5a;
                                                 }
                                                 pre {
                                                     background-color: #f6f8fa;
                                                     border: 1px solid #e1e4e8;
                                                     border-radius: 6px;
                                                     padding: 16px;
                                                     overflow: auto;
                                                     margin: 1em 0;
                                                 }
                                                 pre code {
                                                     background: none;
                                                     border: none;
                                                     padding: 0;
                                                     color: #24292e;
                                                     font-size: 0.9em;
                                                     line-height: 1.45;
                                                 }
                                                 /* 代码高亮颜色 (类似 VSCode) */
                                                 .keyword { color: #d73a49; }        /* 关键字 */
                                                 .string { color: #032f62; }         /* 字符串 */
                                                 .comment { color: #6a737d; }        /* 注释 */
                                                 .number { color: #005cc5; }         /* 数字 */
                                                 .function { color: #6f42c1; }       /* 函数 */
                                                 .class-name { color: #22863a; }     /* 类名 */
                                                 /* 引用块 */
                                                 blockquote {
                                                     background-color: #f6f8fa;
                                                     border-left: 4px solid #dfe2e5;
                                                     margin: 1em 0;
                                                     padding: 0.5em 1em;
                                                     color: #6a737d;
                                                 }
                                                 blockquote p {
                                                     margin: 0.5em 0;
                                                     color: #6a737d;
                                                 }
                                                 /* 列表样式 */
                                                 ul, ol {
                                                     margin: 1em 0;
                                                     padding-left: 2em;
                                                 }
                                                 li {
                                                     margin: 0.5em 0;
                                                     color: #333333;
                                                 }
                                                 ul li {
                                                     list-style-type: disc;
                                                 }
                                                 ol li {
                                                     list-style-type: decimal;
                                                 }
                                                 /* 表格样式 */
                                                 table {
                                                     border-collapse: collapse;
                                                     width: 100%;
                                                     margin: 1em 0;
                                                     border: 1px solid #e1e4e8;
                                                 }
                                                 th {
                                                     background-color: #f6f8fa;
                                                     border: 1px solid #e1e4e8;
                                                     padding: 8px 12px;
                                                     font-weight: 600;
                                                     text-align: left;
                                                     color: #000000;
                                                 }
                                                 td {
                                                     border: 1px solid #e1e4e8;
                                                     padding: 8px 12px;
                                                     color: #333333;
                                                 }
                                                 tr:nth-child(even) {
                                                     background-color: #fafbfc;
                                                 }
                                                 /* 水平线 */
                                                 hr {
                                                     border: none;
                                                     border-top: 1px solid #e1e4e8;
                                                     margin: 2em 0;
                                                 }
                                                 /* 图片 */
                                                 img {
                                                     max-width: 100%;
                                                     height: auto;
                                                     border-radius: 3px;
                                                     border: 1px solid #e1e4e8;
                                                 }
                                                 /* 强调文本 */
                                                 strong {
                                                     color: #000000;
                                                     font-weight: 600;
                                                 }
                                                 em {
                                                     color: #333333;
                                                     font-style: italic;
                                                 }
                                                 /* 任务列表 */
                                                 .task-list-item {
                                                     list-style-type: none;
                                                 }
                                                 .task-list-item input[type="checkbox"] {
                                                     margin: 0 0.5em 0.2em -1.3em;
                                                     vertical-align: middle;
                                                 }
                                                 /* 行内数学公式 */
                                                 .math-inline {
                                                     background-color: #f6f8fa;
                                                     padding: 0.1em 0.3em;
                                                     border-radius: 3px;
                                                     font-family: 'Cambria Math', serif;
                                                 }
                                                 /* 代码块标题 */
                                                 .code-block-title {
                                                     background: #e1e4e8;
                                                     color: #586069;
                                                     padding: 4px 12px;
                                                     font-size: 0.85em;
                                                     font-weight: 600;
                                                     border-radius: 6px 6px 0 0;
                                                     margin-bottom: -1px;
                                                 }
                                                 /* 响应式设计 */
                                                 @media (max-width: 768px) {
                                                     body {
                                                         padding: 15px 20px;
                                                         font-size: 13px;
                                                     }
                                                     h1 { font-size: 1.8em; }
                                                     h2 { font-size: 1.5em; }
                                                     h3 { font-size: 1.2em; }
                                                     pre {
                                                         padding: 12px;
                                                         font-size: 0.85em;
                                                     }
                                                 }
        )EOF");
        html = QString("<!DOCTYPE html><html lang=\"zh-CN\"><head><style>%1</style></head><body>%2</body></html>").arg(lightTheme).arg(html);
        // qInfo() << html;
        ui->textBrowser->document()->setHtml(html);

//        ui->textBrowser->document()->setMarkdown(ui->textEdit->toPlainText());
//        setSingleLineTable(ui->textBrowser);
    });
    connect(ui->textBrowser, &QTextBrowser::anchorClicked, ui->textBrowser, [this](const QUrl& url){
        QString anchor = url.toString();
        qInfo() << anchor.toStdString().c_str() << "anchorClicked";
        if (anchor.startsWith("#")) {
            scrollToHeading(ui->textBrowser, anchor.mid(1));
            // ui->textBrowser->scrollToAnchor(anchor.mid(1));
        }
    });
    Q_EMIT ui->textEdit->textChanged();
}


MainWindow::~MainWindow()
{
    delete ui;
}
