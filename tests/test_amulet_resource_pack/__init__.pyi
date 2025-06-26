from __future__ import annotations

import faulthandler as faulthandler

from . import _test_amulet_resource_pack

__all__ = ["compiler_config", "faulthandler"]

def _init() -> None: ...

compiler_config: dict = {
    "pybind11_version": "2.13.6",
    "compiler_id": "MSVC",
    "compiler_version": "19.43.34808.0",
}
