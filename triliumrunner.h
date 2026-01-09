#ifndef TRILIUMRUNNER_H
#define TRILIUMRUNNER_H

#include <KRunner/AbstractRunner>
#include <KPluginMetaData>

// Forward declarations to keep the header lightweight
namespace KRunner {
  class RunnerContext;
  class QueryMatch;
}

// sqlite3 is an opaque type here; include sqlite3.h only in the .cpp
struct sqlite3;

class TriliumRunner : public KRunner::AbstractRunner {
  Q_OBJECT

  public:
    TriliumRunner(QObject *parent, const KPluginMetaData &metaData);
    ~TriliumRunner() override;

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
  private:
    sqlite3 *m_db = nullptr;
};

#endif