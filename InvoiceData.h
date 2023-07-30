#ifndef INVOICEDATA_H
#define INVOICEDATA_H

#include "InvoiceDetail.h"

#include <vector>

class InvoiceData
{
public:
    InvoiceData();

    int clientId = -1;
    int stylesheetId = -1;
    std::vector<InvoiceDetail> details;
};

#endif // INVOICEDATA_H
