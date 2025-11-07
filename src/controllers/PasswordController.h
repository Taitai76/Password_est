#pragma once
#include <drogon/HttpController.h>

class PasswordController : public drogon::HttpController<PasswordController> {
public:
  METHOD_LIST_BEGIN
    ADD_METHOD_TO(PasswordController::estimate, "/api/estimate", drogon::Post);
  METHOD_LIST_END

  void estimate(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> && cb);
};
