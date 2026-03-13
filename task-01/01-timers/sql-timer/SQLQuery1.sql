SET NOCOUNT ON;

DECLARE @Measurements TABLE (ID INT, T DATETIME2(7));
DECLARE @WarmUpMeasurements TABLE (ID INT, T DATETIME2(7));
DECLARE @Counter INT = 1;
DECLARE @WarmupIterations INT = 1000;
DECLARE @ActualIterations INT = 1000;

---------------------------------------------------------
-- WARM-UP
---------------------------------------------------------
SET @Counter = 1;
WHILE @Counter <= @WarmupIterations
BEGIN
    INSERT INTO @WarmUpMeasurements (ID, T) VALUES (@Counter, SYSDATETIME());
    SET @Counter += 1;
END

SET @Counter = 1;
WHILE @Counter <= @ActualIterations
BEGIN
    INSERT INTO @Measurements (ID, T) VALUES (@Counter, SYSDATETIME());
    SET @Counter += 1;
END

DELETE FROM @Measurements

---------------------------------------------------------
-- ACTUAL TEST
---------------------------------------------------------
SET @Counter = 1;
WHILE @Counter <= @ActualIterations
BEGIN
    INSERT INTO @Measurements (ID, T) VALUES (@Counter, SYSDATETIME());
    SET @Counter += 1;
END

---------------------------------------------------------
-- SHOW RESULTS
-- DATEDIFF_BIG - https://learn.microsoft.com/en-us/sql/t-sql/functions/datediff-big-transact-sql?view=sql-server-ver17
---------------------------------------------------------
SELECT 
    m.ID,
    DATEDIFF_BIG(NANOSECOND, 
                 FIRST_VALUE(w.T) OVER (ORDER BY w.ID), 
                 w.T) AS WarmUp_Elapsed_NS,
    
    DATEDIFF_BIG(NANOSECOND, 
                 FIRST_VALUE(m.T) OVER (ORDER BY m.ID), 
                 m.T) AS Actual_Elapsed_NS

FROM @Measurements m 
JOIN @WarmUpMeasurements w ON m.ID = w.ID
ORDER BY m.ID;