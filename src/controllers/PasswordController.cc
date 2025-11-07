#include "PasswordController.h"
#include "../utils/Strength.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <utility>  // for std::move

void PasswordController::estimate(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && cb) {

  auto json = req->getJsonObject();
  if (!json || !(*json)["password"].isString()) {
    auto res = drogon::HttpResponse::newHttpJsonResponse(
      Json::Value({{"error","Expected JSON {\"password\":\"...\"}"}}));
    res->setStatusCode(drogon::k400BadRequest);
    return std::move(cb)(res);
  }

  const auto pw = (*json)["password"].asString();
  auto result = estimate_strength(pw);

  Json::Value out;
  out["score"] = result.score;
  out["verdict"] = result.verdict;
  out["entropy_bits"] = result.entropy_bits;
  out["feedback"] = Json::arrayValue;
  for (auto& s : result.feedback) out["feedback"].append(s);

  auto res = drogon::HttpResponse::newHttpJsonResponse(out);
  res->addHeader("Access-Control-Allow-Origin","*");
  std::move(cb)(res);
}
