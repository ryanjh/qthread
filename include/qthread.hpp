class Qthread
{
public:
    Qthread();
    virtual ~Qthread();

    void listIpAddr(void);
    void sanityTest(void);
private:
    QThread *system_thread = nullptr;
    void    *instance      = nullptr;
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
