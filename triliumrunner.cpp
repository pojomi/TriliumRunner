#include "triliumrunner.h"
#include <KPluginMetaData>
#include <KPluginFactory>
#include <QtGui/QIcon>
#include <QtCore/QRegularExpression>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QStandardPaths>
#include <qobject.h>
#include <sqlite3.h>

TriliumRunner::TriliumRunner(QObject *parent, const KPluginMetaData &metaData)
  : KRunner::AbstractRunner(parent, metaData)
{
  setObjectName(QStringLiteral("Trilium"));

  QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
  QString dbPath = dataDir + QStringLiteral("/trilium-data/document.db");
  QByteArray dbPathBytes = dbPath.toUtf8();
  const char *c_dbPath = dbPathBytes.constData();
  qDebug() << QStringLiteral("TriliumRunner: opening DB at") << dbPath;

  int rc = sqlite3_open_v2(c_dbPath, &m_db, SQLITE_OPEN_READONLY, NULL);
  if (rc != SQLITE_OK) {
    qWarning() << QStringLiteral("TriliumRunner: failed to open DB:") << sqlite3_errstr(rc);
    m_db = nullptr;
  }
  this->setMinLetterCount(5);
}

TriliumRunner::~TriliumRunner()
{
  if (m_db) {
    sqlite3_close(m_db);
  }
}

void TriliumRunner::match(KRunner::RunnerContext &context)
{
  const QString term = context.query();

  if (!m_db) {
    return;
  }

  QByteArray utf8Term = term.toUtf8();
  char wildcard[256];
  snprintf(wildcard, sizeof(wildcard), "%%%s%%", utf8Term.constData());

  sqlite3_stmt *stmt;
  const char *sql = "SELECT n.title, b.content, n.noteId "
                    "FROM notes n "
                    "JOIN blobs b ON n.blobId = b.blobId "
                    "WHERE (n.title LIKE :term OR b.content LIKE :term) "
                    "AND n.title IS NOT NULL "
                    "ORDER BY n.title LIKE :term DESC LIMIT 10;";
  if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    int idx = sqlite3_bind_parameter_index(stmt, ":term");
    sqlite3_bind_text(stmt, idx, wildcard, -1, SQLITE_STATIC);

    QList<KRunner::QueryMatch> matches;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      QString title = QString::fromUtf8((const char*)sqlite3_column_text(stmt, 0));
      QString content = QString::fromUtf8((const char*)sqlite3_column_text(stmt, 1));
      QString noteId = QString::fromUtf8((const char*)sqlite3_column_text(stmt, 2));

      // Create a KRunner match
      KRunner::QueryMatch match(this);
      match.setText(title);

      // Strip HTML
      content.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
      // Clean up whitespace/newlines
      match.setSubtext(content.simplified().left(100));

      match.setData(noteId);
      match.setIcon(QIcon::fromTheme(QStringLiteral("trilium")));
      match.setRelevance(1.0);
      matches << match;
    }
    sqlite3_finalize(stmt);
    context.addMatches(matches);
  }
  else {
    qWarning() << QStringLiteral("TriliumRunner: failed to prepare statement:") << sqlite3_errmsg(m_db);
  }
}

void TriliumRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
  // Q_UNUSED macro to avoid unused parameter warnings
  Q_UNUSED(context);
  qDebug() << "TriliumRunner::run() called!";

  QString noteId = match.data().toString();
  qDebug() << "TriliumRunner: noteId =" << noteId;

  if (noteId.isEmpty()) {
    qWarning() << QStringLiteral("TriliumRunner: empty noteId, cannot run");
    return;
  }

  if (!QProcess::startDetached(QStringLiteral("/bin/sh"), 
      QStringList() << QStringLiteral("-c") 
                    << QStringLiteral("TRILIUM_START_NOTE_ID=") + noteId + QStringLiteral(" trilium"))) {
    qWarning() << QStringLiteral("TriliumRunner: failed to start Trilium");
  }
  
}
K_PLUGIN_CLASS_WITH_JSON(TriliumRunner, "metadata.json")

#include "triliumrunner.moc"