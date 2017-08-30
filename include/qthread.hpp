class SystemThread; //forward declaration

/**
 * Log levels.
 */
enum class Debug_Level
{
    NORMAL = 0,
    INFO   = 3,
    DEBUG  = 4,
};

class Qthread
{
    friend class SystemThread;

public:
    Qthread();
    explicit Qthread(quint32 node_id);
    virtual ~Qthread();
    Qthread(const Qthread&)            = delete;
    Qthread& operator=(const Qthread&) = delete;
    bool operator==(const Qthread&);

    void enableDebug(Debug_Level level);
    QList<QString> getIpAddress(void);
    QString getGlobalAddress(void);
    QString getRole(void);
    quint16 getRloc(void);

    /**
     * diagnostics
     */
    void sanityTest(void);

    /**
     * icmpv6
     */
    void ping(QString ipAddress, quint16 length = 8);

private:
    SystemThread *system_thread  = nullptr;
    void         *instance       = nullptr;
    quint8       *instanceBuffer = nullptr;
};

/*  /usr/local/src/openthread/examples/platforms/posix/platform-posix.h  */
/**
 * Unique node ID.
 *
 */
extern quint32 NODE_ID;

/**
 * Well-known Unique ID used by a simulated radio that supports promiscuous mode.
 *
 */
extern quint32 WELLKNOWN_NODE_ID;
