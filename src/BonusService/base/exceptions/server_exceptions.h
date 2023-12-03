#pragma once

#include "base_exception.h"

BADOON_EXCEPTION( ServerException, BaseException, "(Server Error) " )
BADOON_EXCEPTION( JsonParserException, ServerException, "(Json parser Error) " )
BADOON_EXCEPTION(
  ConfigReaderException,
  ServerException,
  "(Config read Error) "
)
