//===-- test-gamemodel.cpp - Self-tests for GameModel -------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/model/GameModel.h"

#include "gtest/gtest.h"

#include <iostream>

static void CheckRoundTripSerialize(unsigned B, unsigned W, unsigned T,
                                    bool ClockRunning, GameModel::GameState S) {
  GameModel Src;
  Src.BlackScore = B;
  Src.WhiteScore = W;
  Src.GameClockSecs = T;
  Src.ClockRunning = ClockRunning;
  Src.GS = S;

  std::string Ser = Src.serialize();
  GameModel Dst;

  EXPECT_FALSE(GameModel::deSerialize(Ser, Dst))
    << "(B:" << B << ", W:" << ", T:" << T
    << ", R:" << (ClockRunning?"Y":"N") << ")\n"
    << " Can't deserialize\n";

  EXPECT_FALSE(Dst != Src)
    << "(B:" << B << ", W:" << W << ", T:" << T << ")\n"
    << Dst.dump() << "\n"
    << Src.dump() << "\n"
    << " Doesn't match\n";
}

TEST(TestGameModel, SerializeGameState) {
  CheckRoundTripSerialize(42, 37, 104, false, GameModel::GS_WallClock);
  CheckRoundTripSerialize(42, 37, 104, false, GameModel::GS_FirstHalf);
  CheckRoundTripSerialize(42, 37, 104, false, GameModel::GS_HalfTime);
  CheckRoundTripSerialize(42, 37, 104, false, GameModel::GS_GameOver);
}

TEST(TestGameModel, SerializeScore) {
  for (unsigned B = 0; B < 10; B++)
    for (unsigned W = 0; W < 10; W++)
      for (unsigned T = 0; T < 10; T++) {
        CheckRoundTripSerialize(B, W, T, false, GameModel::GS_WallClock);
        CheckRoundTripSerialize(B, W, T, true,  GameModel::GS_WallClock);
      }
}

TEST(TestGameModel, SerializeEdgeCases) {
  CheckRoundTripSerialize(99, 99, 9999, true, GameModel::GS_WallClock);
  CheckRoundTripSerialize(99, 99, 999999, false, GameModel::GS_WallClock);
  CheckRoundTripSerialize(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, true, GameModel::GS_WallClock);
}

static void CheckNotEquals(GameModel M1, GameModel M2) {
  EXPECT_FALSE(M1 == M2)
    << M1.serialize()
    << " == "
    << M2.serialize()
    << " operator==()";

  EXPECT_FALSE(!(M1 != M2))
    << M1.serialize()
    << " == "
    << M2.serialize()
    << " operator!=()";

  EXPECT_FALSE(M1.BlackScore == M2.BlackScore &&
               M1.WhiteScore == M2.WhiteScore &&
               M1.ClockRunning == M2.ClockRunning &&
               ((M1.ClockRunning && M1.MK != GameModel::MK_PassiveSlave) ||
                M1.GameClockSecs == M2.GameClockSecs) &&
               M1.GS == M2.GS &&
               M1.TS == M2.TS)
    << M1.serialize()
    << " == "
    << M2.serialize()
    << " equality\n";
}

static void CheckEquals(GameModel M1, GameModel M2) {
  EXPECT_FALSE(M1 != M2)
    << M1.serialize()
    << " != "
    << M2.serialize()
    << " operator!=()";

  EXPECT_FALSE(!(M1 == M2))
    << M1.serialize()
    << " != "
    << M2.serialize()
    << " operator==()";

  EXPECT_FALSE(M1.BlackScore != M2.BlackScore ||
               M1.WhiteScore != M2.WhiteScore ||
               M1.ClockRunning != M2.ClockRunning ||
               ((!M1.ClockRunning && M1.MK == GameModel::MK_PassiveSlave) &&
                M1.GameClockSecs != M2.GameClockSecs) ||
               M1.GS != M2.GS ||
               M1.TS != M2.TS)
    << M1.serialize()
    << " != "
    << M2.serialize()
    << " inequality\n";
}

TEST(TestGameModel, Equality) {
  GameModel M1;
  M1.BlackScore = 1;
  M1.WhiteScore = 2;
  M1.GameClockSecs = 15;
  M1.ClockRunning = true;
  M1.GS = GameModel::GS_WallClock;
  M1.MK = GameModel::MK_PassiveSlave;

  GameModel M2 = M1;
  CheckEquals(M1, M2);
  CheckEquals(M2, M1);

  M2 = M1;
  M2.WhiteScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.BlackScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.WhiteScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.BlackScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.WhiteScore = 5;
  M2.BlackScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.WhiteScore = 5;
  M2.BlackScore = 4;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);

  M2 = M1;
  M2.GS = GameModel::GS_SecondHalf;
  CheckNotEquals(M1, M2);
  CheckNotEquals(M2, M1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
