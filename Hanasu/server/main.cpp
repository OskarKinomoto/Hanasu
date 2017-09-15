//#include <Socket.h>

#include <Log.h>
#include <Socket.h>
#include <Protocol.hpp>
#include <ErrnoException.h>
#include <Serialize.hpp>
#include <hash.hpp>

#include <signal.h>

#include <iostream>
#include <thread>
#include <vector>


#include "DB.hpp"
#include "Session.hpp"

namespace {

bool run = true;

std::shared_ptr<DB> db;

}

void kill_handler(int) {
    run = false;
}

void runMe(Socket &&soc) {;

    try {
        Session cs{std::move(soc), db};
        cs.run();

    } catch (const ErrnoException &e) {
        if (e.err == EPIPE) {
            LOGD("Client closed connection!");
        } else if (e.err == EINTR) {
            LOGD("Client cosed connection!");
        } else {
            LOGE(e.what());
        }
    } catch (const std::exception &e) {
        LOGE(e.what());
    } catch (...) {
        LOGE("Unknown error!!!");
    }
}

int main() {

    std::vector<std::thread> threads{};

    try {
        signal(SIGTERM, kill_handler);

        db = std::make_shared<DB>();

        Socket soc(Domain::INet6, Type::Stream);
        soc.bind(8080);
        soc.listen(10);

        while (run) {
            if (soc.pollin(100)) {
                auto clientSoc = soc.accept();
                threads.push_back(std::thread(runMe, std::move(clientSoc)));
            }
        }

        Session::stopAll();

    } catch (std::exception& e) {
        LOGE(e.what());
    } catch (...) {
        LOGE("Unknown error!!!");
    }

    Session::stopAll();

    for (auto& thread : threads)
        thread.join();

    return 0;
}
