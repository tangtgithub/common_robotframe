#ifndef __HTTP_CB_H__
#define __HTTP_CB_H__

#include <memory>
#include "commondefine.h"
#include "HttpCallback.h"

class HttpCb :public HttpCallback
{
public:
    void init() override;
    void start() override;


};

#endif /* __HTTP_CB_H__ */
