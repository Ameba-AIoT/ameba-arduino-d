#include "Arduino.h"
#include "AmebaMDNS.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mDNS.h"

#ifdef __cplusplus
}
#endif

void MDNSClass::begin() {
    mDNSResponderInit();
}

void MDNSClass::end() {
    mDNSResponderDeinit();
}

void MDNSClass::registerService(MDNSService service) {
    service.serviceid = mDNSRegisterService(service.name, service.service_type, service.domain, service.port, ((TXTRecordRef *)(service.txtRecordObj)));
}

void MDNSClass::deregisterService(MDNSService service) {
    mDNSDeregisterService(service.serviceid);
}

void MDNSClass::updateService(MDNSService service, unsigned int ttl) {
    mDNSUpdateService(service.serviceid, ((TXTRecordRef *)(service.txtRecordObj)), ttl);
}

MDNSService::MDNSService(char *name, char *service_type, char *domain, unsigned short port, int bufsize) {
    this->name = (char *)(malloc(strlen(name) + 1));
    sprintf(this->name, "%s", name);

    this->service_type = (char *)(malloc(strlen(service_type) + 1));
    sprintf(this->service_type, "%s", service_type);

    this->domain = (char *)malloc((strlen(domain) + 1));
    sprintf(this->domain, "%s", domain);

    this->port = port;

    buf = (char *)(malloc(bufsize));
    memset(buf, 0, bufsize);

    TXTRecordCreate(((TXTRecordRef *)txtRecordObj), bufsize, buf);

    serviceid = NULL;
}

MDNSService::~MDNSService() {
    if (name != NULL) {
        free(name);
        name = NULL;
    }

    if (service_type != NULL) {
        free(service_type);
        service_type = NULL;
    }

    if (domain != NULL) {
        free(domain);
        domain = NULL;
    }

    port = 0;

    TXTRecordDeallocate(((TXTRecordRef *)txtRecordObj));

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
}

int MDNSService::addTxtRecord(char *key, int value_len, char *value) {
    return TXTRecordSetValue(((TXTRecordRef *)txtRecordObj), key, value_len, value);
}

MDNSClass MDNS;
