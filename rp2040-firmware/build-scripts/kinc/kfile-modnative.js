let project = new Project('ng6502');
const path = require('path');

project.cpp = false;
project.cStd = "c99"

//await project.addProject('/home/ttrocha/_dev/extprojects/incubation/kinc/Kinc');
await project.addProject('tmp/Kinc');
project.addDefine('__KINC__');
project.addDefine('SOUND');

// use the mod directly
project.addDefine('_MOD_NATIVE_');
project.addIncludeDir('../../source/api')
project.addIncludeDir('../../mod-flappy')
project.addIncludeDir('../../source')

project.addDefine('INCLUDE_DATA');

//project.addDefine('DATA_FROM_MEMORY');
project.addFile('../../source/*.c');
project.addFile('../../source/3rd/modplay/*.c');
project.addFile('../../source/core/*.c');
//project.addFile('../../source/game/*.c');
project.addFile('../../source/core/backend/kinc/*.c');

// MOD - NATIVE - FILE
//project.addFile('../../mod/source/mod_pixelbuffer.c');
//project.addFile('../../mod/source/mod_simple.c');
project.addFile('../../mod-flappy/source/*.c');

project.addFile('../../source/api/*.c');
//project.addFile('../../test/gen_prg1.c')
//project.addFile('../../mod/export/*.c')
project.setDebugDir('Deployment');

project.version = '1.0.0';
project.id = '7d80aa44-6ce4-4e02-8857-4703a02d0bae';

project.targetOptions.switch.version = '1.0.00';
project.targetOptions.switch.releaseVersion = '0';
project.targetOptions.switch.applicationId = '0x01004b9000490000';
project.targetOptions.switch.publisher = 'Rob';

project.targetOptions.xboxSeriesXS.identityName = 'ROB.0000000000000';
project.targetOptions.xboxSeriesXS.identityPublisher = '00000000-0000-0000-0000-000000000000';
project.targetOptions.xboxSeriesXS.publisher = 'ROB';
project.targetOptions.xboxSeriesXS.titleId = '00000000';
project.targetOptions.xboxSeriesXS.configId = '00000000-0000-0000-0000-000000000000';
project.targetOptions.xboxSeriesXS.storeId = '000000000000';
project.targetOptions.xboxSeriesXS.version = '1.0.0.31';


const sony = 'SIEE';
const sonyPatch = false;

project.targetOptions.playStation5.department = sony;
project.targetOptions.playStation5.patch = sonyPatch;

project.targetOptions.playStation5.sieeMainMasterVersion = '01.00'; // increase for re-submission, reset for new version
project.targetOptions.playStation5.sieeMainContentVersion = '01.000.000'; // increase for new version
project.targetOptions.playStation5.sieeAppPath = path.resolve('MasterBuilds', 'PS5-SIEE', 'whatever', 'unused.pkg');
project.targetOptions.playStation5.sieePatchMasterVersion = '01.00';
project.targetOptions.playStation5.sieePatchContentVersion = '01.006.000';
project.targetOptions.playStation5.sieeId = '000000-000000000_00-0000000000000000';
project.targetOptions.playStation5.sieePasscode = 'arrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr';

project.flatten();

resolve(project);
