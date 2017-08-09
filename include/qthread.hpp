class SystemThread; //forward declaration

class Qthread
{
    friend class SystemThread;

public:
    Qthread();
    explicit Qthread(uint32_t node_id);
    virtual ~Qthread();
    Qthread(const Qthread&)            = delete;
    Qthread& operator=(const Qthread&) = delete;

    void listIpAddr(void);
    void sanityTest(void);

private:
    SystemThread *system_thread  = nullptr;
    void         *instance       = nullptr;
    uint8_t      *instanceBuffer = nullptr;
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
