#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "application.h"

int main(int argc, char* argv[])
{
       std::unique_ptr<Application> app(std::make_unique<Application>());
        app->Initialize();
}
