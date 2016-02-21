/* 
 * File:   bender_face.h
 * Author: benderteam
 *
 * Created on 7 de marzo de 2015, 02:35 AM
 */

// XXX: marca fara cosas que no se sabe aún si son útiles

#ifndef BENDER_FACE_H
#define	BENDER_FACE_H

#ifdef	__cplusplus
extern "C" {
#endif

    // XXX save app config
    #define SaveAppConfig(a)
    
    // XXX: app config validation
    // Define a header structure for validating the AppConfig data structure in EEPROM/Flash
    typedef struct
    {
            unsigned short wConfigurationLength;	// Number of bytes saved in EEPROM/Flash (sizeof(APP_CONFIG))
            unsigned short wOriginalChecksum;		// Checksum of the original AppConfig defaults as loaded from ROM (to detect when to wipe the EEPROM/Flash record of AppConfig due to a stack change, such as when switching from Ethernet to Wi-Fi)
            unsigned short wCurrentChecksum;		// Checksum of the current EEPROM/Flash data.  This protects against using corrupt values if power failure occurs while writing them and helps detect coding errors in which some other task writes to the EEPROM in the AppConfig area.
    } NVM_VALIDATION_STRUCT;


    typedef enum {
        EMOTION_SERIOUS,
        EMOTION_HAPPY1, EMOTION_HAPPY2, EMOTION_HAPPY3,
        EMOTION_SAD1,   EMOTION_SAD2,   EMOTION_SAD3,
        EMOTION_ANGRY1, EMOTION_ANGRY2, EMOTION_ANGRY3,
        EMOTION_SURPRISED,
        EMOTION_ASHAMED,
        EMOTION_STANDBY,
        EMOTION_EYEBROW,
        EMOTION_GREETINGS,
        EMOTION_1313,
        EMOTION_EAR,
        EMOTION_YES,
        EMOTION_NO,
        EMOTION_LOST,
        EMOTION_RELAXED,
        EMOTION_NORMAL,
        EMOTION_AGITATED,
        EMOTION_BLINK,
        EMOTION_FLIRT,
        EMOTION_NONE    // no emotion selected
    } BENDER_EMOTIONS;


    // XXX: head struct TODO
    typedef struct
    {
        BENDER_EMOTIONS emotion;
        int neckYaw;
        BOOL mouthEnabled;
    } bHeadState;
    #define BENDER_NECK_MAX_YAW ( 45 )
    #define BENDER_NECK_MIN_YAW (-45 )

    void initHeadState();
    BYTE* getHeadEmotionStr();
    BOOL isMouthEnabled();
    int getNeckYaw();
    void setNeckYaw(int yaw);
    void setEmotion(BYTE* emotion);
    void setMouthState(BYTE* state);

#ifdef	__cplusplus
}
#endif

#endif	/* BENDER_FACE_H */

