#include "oatpp/OatppServer.h"
#include "Factory/PGFactory.h"
#include "BLFacade.h"
#include <boost/di.hpp>
#include <config/yamlcpp_config.h>
#include "oatpp/core/macro/component.hpp"
int main(int argc, const char* argv[]) {
    if (argc < 2)
        return 1;
    std::string CONFIG_PATH = argv[1]; // "/Users/kate/Desktop/ds-lsb2/src/BonusService/Configs/Config.yml";//

    auto injector = di::make_injector(
            di::bind<BaseConfig>().to<YamlCppConfig>(),
            di::bind<std::string>.named( configFileName ).to( CONFIG_PATH ),
            di::bind<IDAFactory>().to<PGDAFactory>(),
            di::bind<IBLFacade>().to<BLFacade>(),
            di::bind<IServer>().to<OatppServer>());

    auto server = injector.create<IServerPtr>();
    LoggerFactory::InitLogger( injector.create<BaseConfigPtr>());

    server->run();

    return 0;
}