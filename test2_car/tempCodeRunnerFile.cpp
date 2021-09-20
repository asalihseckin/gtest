    ON_CALL(meb, getSpeeed(60)).WillByDefault(Return(60));
    ON_CALL(meb, getRoadDistance(180)).WillByDefault(Return(1800));