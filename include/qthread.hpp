class SystemThread; //forward declaration

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

    void sanityTest(void);
    QList<QString> getIpAddress(void);
    QString getRole(void);
    quint16 getRloc(void);

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
