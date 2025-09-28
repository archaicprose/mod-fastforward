DROP TABLE IF EXISTS `ff_character_prep`;
CREATE TABLE `ff_character_prep` (
	`account_id` INT NOT NULL COMMENT 'Account id from acore_auth.account',
	`spec` TINYINT NOT NULL DEFAULT '0' COMMENT 'Character expected class & intended spec',
    `level` TINYINT NOT NULL DEFAULT '1' COMMENT 'Character intended level',
    `stage` TINYINT NOT NULL DEFAULT '0' COMMENT 'Character intended raiding stage',
    `claimed` TINYINT NOT NULL DEFAULT '0' COMMENT 'Whether this prep has been claimed',
    `created_on` DATETIME COMMENT 'Used to calculate expiration',
    -- TODO : skills, etc
    INDEX (`claimed`, `created_on`)
);
