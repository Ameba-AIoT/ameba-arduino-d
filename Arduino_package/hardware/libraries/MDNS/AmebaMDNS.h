#ifndef _MDNS_H_
#define _MDNS_H_

class MDNSClass;
class MDNSService;

class MDNSClass {
    public:
        void begin();
        void end();
        void registerService(MDNSService service);
        void deregisterService(MDNSService service);
        void updateService(MDNSService service, unsigned int ttl);
};

class MDNSService {
    public:
        MDNSService(char *name, char *service_type, char *domain, unsigned short port, int txtbufsize = 128);
        ~MDNSService();

        int addTxtRecord(char *key, int value_len, char *value);
        friend class MDNSClass;

    private:
        char *name;
        char *service_type;
        char *domain;
        unsigned short port;
        char *buf;
        char *txtRecordObj[16];
        void *serviceid;
};

extern MDNSClass MDNS;
#endif
