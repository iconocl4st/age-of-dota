
add_library(common_lib STATIC
        src/common/state/quad_tree/QuadTree.cpp
        src/common/state/quad_tree/QuadTree.h
        src/common/state/quad_tree/QuadTreeImpl.h
        src/common/state/quad_tree/QuadTreeNode.h
        src/common/state/quad_tree/QuadTreeLeafNode.h
        src/common/state/quad_tree/QuadTreeBranchNode.h

        src/common/state/quad_grid/QuadGrid.cpp
        src/common/state/quad_grid/QuadGrid.h
        src/common/util/ThreadPool.cpp
        src/common/util/ThreadPool.h
        src/common/json/JsonDebuggable.cpp
        src/common/json/JsonDebuggable.h

        src/common/json/JsonTextWriter.cpp
        src/common/json/JsonTextReader.cpp
        src/common/json/JsonSplitWriter.cpp
        src/common/json/JsonTokener.cpp
        src/common/json/JsonTokener.h
        src/common/json/LookAhead.cpp
        src/common/json/LookAhead.h

        src/common/network/ConnectionBuffer.cpp
        src/common/network/ConnectionBuffer.h
        src/common/network/MessageTypes.h
        src/common/network/DefaultMessageHandler.cpp
        src/common/network/DefaultMessageHandler.h
        src/common/network/ConnectionContext.cpp
        src/common/network/ConnectionContext.h
        src/common/network/MessageTypes.cpp
        src/common/network/BufferWrapper.cpp
        src/common/network/BufferWrapper.h
        src/common/network/BufferManager.cpp
        src/common/network/BufferManager.h

        src/common/state/ResourceMap.cpp
        src/common/state/ResourceMap.h

        src/common/state/definitions.h

        src/common/state/state_manipulator.cpp
        src/common/state/state_manipulator.h
        src/common/state/Entity.cpp
        src/common/state/Entity.h

        src/common/spec/EntitySpec.cpp
        src/common/spec/EntitySpec.h
        src/common/spec/WeaponSpec.cpp
        src/common/spec/WeaponSpec.h
        src/common/spec/GameSpecification.cpp
        src/common/spec/GameSpecification.h
        src/common/state/Actions.h
        src/common/state/Shape.cpp
        src/common/state/Shape.h
        src/common/json/JsonTextReader.h
        src/common/json/JsonSplitWriter.h
        src/common/json/JsonTextWriter.h
        src/common/network/CloseOnExit.cpp
        src/common/network/CloseOnExit.h
        src/common/util/Timer.cpp
        src/common/util/Timer.h
        src/common/util/RandomGenerator.cpp
        src/common/util/RandomGenerator.h


        src/common/state/LocationManager.cpp
        src/common/state/LocationManager.h
        src/common/state/CommonState.cpp
        src/common/state/CommonState.h
        src/common/network/sockets.h

        src/common/network/PlayerLaunchOptions.cpp
        src/common/network/PlayerLaunchOptions.h
        src/common/state/Point.cpp
        src/common/state/Point.h
        src/common/json/JsonSendable.cpp
        src/common/json/JsonSendable.h
        src/common/json/JsonReader.h
        src/common/json/JsonWriter.h
        src/common/json/JsonException.cpp
        src/common/network/MessageHandler.h
        src/common/network/MessageHandlingArgs.h
        src/common/network/handle_messages.cpp
        src/common/network/handle_messages.h
        src/common/network/send_noop.cpp
        src/common/network/send_noop.h
        src/common/util/angles.cpp
        src/common/util/angles.h
        src/common/spec/WeaponFire.cpp
        src/common/spec/WeaponFire.h
        src/common/spec/damage_types.h
        src/common/spec/DamageInstance.cpp
        src/common/spec/DamageInstance.h
        src/common/state/manipulation_classes.cpp
        src/common/state/manipulation_classes.h

        src/common/state/quad_tree2/QuadTree2.cpp
        src/common/state/quad_tree2/QuadTree2.h
        src/common/state/quad_tree2/QuadTreeNode2.h
        src/common/state/quad_tree2/QuadTreeBranchNode2.cpp
        src/common/state/quad_tree2/QuadTreeBranchNode2.h
        src/common/state/quad_tree2/QuadTreeLeafNode2.cpp
        src/common/state/quad_tree2/QuadTreeLeafNode2.h
        src/common/state/quad_tree2/QuadTreeNode2.cpp
        src/common/state/quad_tree2/QuadTreePath.cpp
        src/common/state/quad_tree2/QuadTreePath.h

        src/common/network/GameResult.cpp
        src/common/network/GameResult.h
        src/common/util/logging_levels.h
        src/common/util/signal_handler.cpp
        src/common/util/signal_handler.h
        src/common/state/Actions.cpp

        src/common/spec/player_types.h

        src/common/state/quad_tree2/NearestEntitySearchResults.cpp
        src/common/state/quad_tree2/NearestEntitySearchResults.h

        src/common/state/quad_grid/QuadGridNode.cpp
        src/common/state/quad_grid/QuadGridNode.h
        src/common/state/quad_grid/QuadGridBranchNode.cpp
        src/common/state/quad_grid/QuadGridBranchNode.h
        src/common/state/quad_grid/QuadGridLeafNode.cpp
        src/common/state/quad_grid/QuadGridLeafNode.h
        src/common/state/quad_grid/QuadGridFillerNode.cpp
        src/common/state/quad_grid/QuadGridFillerNode.h
        src/common/state/quad_grid/branch_direction.h

        src/common/state/quad_grid/branch_directions.cpp

)