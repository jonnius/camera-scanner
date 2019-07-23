import QtQuick 2.9
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import "./components"
import ImageProcessing 1.0

Page {
    id: settingsPage
    anchors.fill: parent

    header: PageHeader {
        id: header
        title: i18n.tr('General Camera Scanner Settings')
        StyleHints {
            foregroundColor: fgColor
            backgroundColor: bgColor
            dividerColor.visible: false
        }
    }
    
    Column {
        width: parent.width
        spacing: units.gu(2)
        
        anchors.top: header.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        ListItemLayout {
            id: colorModeSwitch
            title.text: i18n.tr("Color Mode")
            title.color: "black"

            Switch {
                id: colorSwitch
                onCheckedChanged: ImageProcessing.config.setColorMode(checked)
            }
        }

        ListItemLayout {
            id: filterModeSwitch
            title.text: i18n.tr("Filter Mode")
            title.color: "black"

            Switch {
                id: filterSwitch
                onCheckedChanged: ImageProcessing.config.setFilterMode(checked)
            }
        }

        Row {
            id: thrRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: units.gu(2)
        
            property var sliderWidth: parent.width - thrLabel.width - units.gu(6)
            
            Label {
                id: thrLabel
                width: text.width
                wrapMode: Text.Wrap
                text: i18n.tr("Color Thr")
            }
            
            Slider {
                id: colorThrSlider
                live: false
                minimumValue: 100.0
                maximumValue: 130.0
                value: ImageProcessing.config.colorThr

                width: thrRow.sliderWidth
                height: units.gu(2)
                
                onPressedChanged: {
                  if(!pressed) {
                    ImageProcessing.config.setColorThr(value)
                  }
                }
            }
        }
        
        Row {
            id: gainRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: units.gu(2)
        
            property var sliderWidth: parent.width - gainLabel.width - units.gu(6)
            
            Label {
                id: gainLabel
                width: text.width
                wrapMode: Text.Wrap
                text: i18n.tr("Color Gain")
            }
            
            Slider {
                id: colorGainSlider
                live: false
                minimumValue: -3.0
                maximumValue: +3.0
                value: ImageProcessing.config.colorGain

                width: gainRow.sliderWidth
                height: units.gu(2)
                
                onPressedChanged: {
                  if(!pressed) {
                    ImageProcessing.config.setColorGain(value)
                  }
                }
            }
        }

        Row {
            id: biasRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: units.gu(2)
        
            property var sliderWidth: parent.width - biasLabel.width - units.gu(6)
            
            Label {
                id: biasLabel
                width: text.width
                wrapMode: Text.Wrap
                text: i18n.tr("Color Bias")
            }
            
            Slider {
                id: colorBiasSlider
                live: false
                minimumValue: -3.0
                maximumValue: +3.0
                value: ImageProcessing.config.colorBias

                width: biasRow.sliderWidth
                height: units.gu(2)
                
                onPressedChanged: {
                  if(!pressed) {
                    ImageProcessing.config.setColorBias(value)
                  }
                }
            }
        }
        
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: i18n.tr("Load Default")
            color: UbuntuColors.green
            onClicked: ImageProcessing.config.loadDefault()            
        }
    }

    Binding {
      target: colorSwitch
      property: "checked"
      value: ImageProcessing.config.colorMode
    }
  
    Binding {
      target: filterSwitch
      property: "checked"
      value: ImageProcessing.config.filterMode
    }
    
    Binding {
      target: colorThrSlider
      property: "value"
      value: ImageProcessing.config.colorThr
      when: !colorThrSlider.pressed
    }
    
    Binding {
      target: colorGainSlider
      property: "value"
      value: ImageProcessing.config.colorGain
      when: !colorGainSlider.pressed
    }
    
    Binding {
      target: colorBiasSlider
      property: "value"
      value: ImageProcessing.config.colorBias
      when: !colorBiasSlider.pressed
    }

}
