from typing import Optional, Iterator, TypeVar, Generic
import json

from amulet.core.block import Block, BlockStack
from amulet.resource_pack.mesh.block import (
    BlockMesh,
    merge_block_meshes,
    get_unit_cube,
)
from amulet.resource_pack.abc.resource_pack import BaseResourcePack
from amulet.utils.image import missing_no_icon_path
from amulet.utils.task_manager import AbstractProgressManager, VoidProgressManager

PackT = TypeVar("PackT", bound=BaseResourcePack)


class BaseResourcePackManager(Generic[PackT]):
    """The base class that all resource pack managers must inherit from. Defines the base api."""

    def __init__(self) -> None:
        self._packs: list[PackT] = []
        self._missing_block: Optional[BlockMesh] = None
        self._texture_is_transparent: dict[str, tuple[float, bool]] = {}
        self._cached_models: dict[BlockStack, BlockMesh] = {}

    @property
    def pack_paths(self) -> list[str]:
        return [pack.root_dir for pack in self._packs]

    def _unload(self) -> None:
        """Clear all loaded resources."""
        self._texture_is_transparent.clear()
        self._cached_models.clear()

    def _load_transparency_cache(self, path: str) -> None:
        try:
            with open(path) as f:
                self._texture_is_transparent = json.load(f)
        except:
            pass

    def _load(self, progress_manager: AbstractProgressManager) -> None:
        """Load resources."""
        raise NotImplementedError

    def reload(
        self, progress_manager: AbstractProgressManager = VoidProgressManager()
    ) -> None:
        """Unload and reload resources"""
        self._unload()
        self._load(progress_manager)

    @property
    def missing_no(self) -> str:
        """The path to the missing_no image"""
        return missing_no_icon_path

    @property
    def missing_block(self) -> BlockMesh:
        if self._missing_block is None:
            texture_path = self.get_texture_path("minecraft", "missing_no")
            self._missing_block = get_unit_cube(
                texture_path,
                texture_path,
                texture_path,
                texture_path,
                texture_path,
                texture_path,
            )
        return self._missing_block

    @property
    def textures(self) -> tuple[str, ...]:
        """Returns a tuple of all the texture paths in the resource pack."""
        raise NotImplementedError

    def get_texture_path(self, namespace: Optional[str], relative_path: str) -> str:
        """Get the absolute texture path from the namespace and relative path pair"""
        raise NotImplementedError

    def get_block_model(self, block_stack: BlockStack) -> BlockMesh:
        """Get a model for a block state.
        The block should already be in the resource pack format"""
        if block_stack not in self._cached_models:
            if len(block_stack) == 1:
                self._cached_models[block_stack] = self._get_model(
                    block_stack.base_block
                )
            else:
                self._cached_models[block_stack] = merge_block_meshes(
                    (self._get_model(block_stack.base_block),)
                    + tuple(
                        self._get_model(block_) for block_ in block_stack.extra_blocks
                    )
                )

        return self._cached_models[block_stack]

    def _get_model(self, block: Block) -> BlockMesh:
        raise NotImplementedError
