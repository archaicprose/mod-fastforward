DELETE FROM `command` WHERE name IN ('ff', 'ff prep', 'ff cancel', 'ff prepfor', 'ff cancelfor');

INSERT INTO `command` (`name`,`security`,`help`) VALUES
	('ff', 0, 'Syntax: .ff $subcommand\nType .help ff to see a list of subcommands\nor .help ff $subcommand to see info on the subcommand.'),
    ('ff prep', 0, 'Syntax: .ff prep $spec $level \nPrep the next fast-forward character for the account.'),
    ('ff cancel', 0, 'Syntax: .ff cancel \nCancel all preparation for the account.'),
    ('ff prepfor', 2, 'Syntax: .ff prepfor $accountname $spec $level \nPrep the next fast-forward character for the specified account.'),
    ('ff cancelfor', 2, 'Syntax: .ff cancel $accountname \nCancel all preparation for the account.');