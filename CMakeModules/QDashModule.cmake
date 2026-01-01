
function(QDashModule library)
    target_link_libraries(${library} PRIVATE ${QDASH_QT_LIBRARIES})
endfunction()
