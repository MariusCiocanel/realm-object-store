////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#ifndef REALM_TEST_UTIL_TEST_FILE_HPP
#define REALM_TEST_UTIL_TEST_FILE_HPP

#include "shared_realm.hpp"

#include <realm/sync/client.hpp>
#include <realm/sync/server.hpp>
#include <realm/util/logger.hpp>

namespace realm {
namespace _impl {
    class AdminRealmManager;
}
}

struct TestFile : realm::Realm::Config {
    TestFile();
    ~TestFile();
};

struct InMemoryTestFile : TestFile {
    InMemoryTestFile();
};

struct SyncTestFile : TestFile {
    SyncTestFile(realm::_impl::AdminRealmManager& manager, realm::StringData id, realm::StringData name);
};

void advance_and_notify(realm::Realm& realm);

#define TEST_ENABLE_SYNC_LOGGING 0 // change to 1 to enable logging

struct TestLogger : realm::util::Logger::LevelThreshold, realm::util::Logger {
    void do_log(realm::util::Logger::Level, std::string) override {}
    Level get() const noexcept override { return Level::off; }
    TestLogger() : Logger::LevelThreshold(), Logger(static_cast<Logger::LevelThreshold&>(*this)) { }

    static realm::sync::Server::Config server_config();
    static realm::sync::Client::Config client_config();
};

class SyncServer {
public:
    SyncServer();
    ~SyncServer();

    std::string url_for_realm(realm::StringData realm_name) const;
    std::string base_url() const { return m_url; }

private:
    realm::sync::Server m_server;
    std::thread m_thread;
    std::string m_url;
};

#endif
