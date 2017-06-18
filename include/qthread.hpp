class Qthread
{
public:
    Qthread();
    virtual ~Qthread();
private:
    QThread *system_thread;
    static uint32_t internal_nodeId;
};

/*  /usr/local/src/openthread/examples/platforms/posix/platform-posix.h  */
/**
 * Unique node ID.
 *
 */
extern uint32_t NODE_ID;

/**
 * Well-known Unique ID used by a simulated radio that supports promiscuous mode.
 *
 */
extern uint32_t WELLKNOWN_NODE_ID;
