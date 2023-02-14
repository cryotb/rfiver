# Intro
Back when i started playing R5R, there was a genuine lack of open-source trainers for it. This is why RFiver has been brought to life, and i've been working on it ever since, until recently.
Due to lack of interest, the project has stalled for a long time already and as of now, nobody is working on it anymore. This is why i decided to open source it.
<br/>
# Objective
The intent of this project was to make cheating in R5R more accessible. Although the game had no anti-cheat, not many open-source trainers are available for it. I did try some public R5R trainers prior, but none of them had the functionality i looked for.<br>
RFiver comes with LOTS of features, ready for use. Yes, most of them are experimental, but still super fun to use!<br>
# Features
- Aim
   - Types: Assisted, Rage.
   - Modes: Plain,Silent,PSilent (packet based silent-aim, also fooling spectators).
   - Adjustable aim smoothing in case of "Plain" mode usage.
   - Adjustable max FOV, Distance.
   - Sync bullets to aimpos (Spectator-like effect on client)
   - [EXP] Trigger (barely works, only used it to test some traceray stuff).
- ESP
   - For: Player, Dummies.
   - Name
   - Box
   - Health/Armor
   - Head pos
   - Sight line
   - Bones
   - Always visible dummy health bar (using game's own renderer, not manually rendered)
   - Glow (static style, quite bad)
- Visual
   - [EXP] Legendary model-changer for weapons (local only)
   - [EXP] Variable Recoil Control
   - FOV Changer (View & ViewModel)
   - Spectator List
     - Spectate other players
   - Player List
   - Thirdperson
   - Viewmodel glow (static style, using scripts)
   - Anti-OBS ( stream proof visuals, thanks to gamesdk.dll )
- Other
   - Hide your own name
   - Anonymize other player names
   - Ignore "Disconnect" StringCmd (partially evades high-ping kicks, blocks some input most of the time)
   - Disable engine tab-out throttle
   - [host-only] Infinite ammo
   - [host-only] Disable damage targeted towards players
   - [EXP] execute commands on the game's squirrel VM (improvised, should be reworked ideally)
# Media
[![IMAGE ALT TEXT](http://img.youtube.com/vi/b4aU8pLgeDE/0.jpg)](http://www.youtube.com/watch?v=b4aU8pLgeDE "Video Title")
# Disclaimer
The aim of this project is to aid people in research of R5R inner workings, production level code is out of question.<br>
Patches released by the R5R dev team might break some things, but the only thing i witnessed breaking was my signatures on gamesdk.dll.<br><br>
The actual game binary pretty much never updated, which is also why i decided to hardcode most offsets.<br>
For using this ingame i recommend using a low FOV with psilent enabled, they are rather quick with banning you if some sweat reports you in their discord channel!<br><br> Beware they also employ **IP-bans**, as an addition to blacklisting your account.<br/>
Lastly i want to mention this trainer is by no means stealth. It's only easy to cheat in R5R because the devs don't care.<br>
# References
- https://unknowncheats.me/forum/apex-legends
- https://www.unknowncheats.me/forum/apex-legends/370655-apex-internal-cheat-huoji.html
- https://github.com/TsudaKageyu/minhook (inline hooking helper, viable in this case, because no anti-cheat)
- https://github.com/nlohmann/json
- https://github.com/fmtlib/fmt
- Various other github repositories.
