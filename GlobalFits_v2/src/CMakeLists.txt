add_library (DatasetLib MiniBooNE.cc PROSPECT.cc CDHS.cc MiniBooNE_combined.cc DANSS.cc NEOS.cc FromChi2Surf.cc Atm.cc MINOS.cc NuMI.cc CCFR.cc Bugey.cc XSec.cc NOMAD.cc KARMEN.cc MiniBooNE_dis.cc LSND_loglikelihood.cc Gallium.cc OscTools.cc XMLTools.cc)

include_directories ( ${CMAKE_SOURCE_DIR}/inc)
include_directories ( ${CMAKE_SOURCE_DIR}/tinyxml)

target_link_libraries(DatasetLib ${ROOT_LIBRARIES})
target_link_libraries(DatasetLib ${GSL_LIBRARIES})
