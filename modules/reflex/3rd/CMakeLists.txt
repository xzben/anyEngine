if(NOT TARGET jsoncpp)
	include(jsoncpp.cmake)
	set_target_properties(jsoncpp PROPERTIES FOLDER ${third_party_folder}/jsoncpp)
endif()