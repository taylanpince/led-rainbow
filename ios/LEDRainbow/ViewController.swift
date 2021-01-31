//
//  ViewController.swift
//  LEDRainbow
//
//  Created by Taylan Pince on 2021-01-29.
//

import UIKit
import CoreBluetooth


enum DeviceMode: UInt8, CustomStringConvertible {
    case automatic = 100
    case alwaysOn = 101
    case alwaysOff = 102
    
    var description: String {
        switch self {
        case .automatic: return "Automatic"
        case .alwaysOn: return "Always On"
        case .alwaysOff: return "Always Off"
        }
    }
}

enum AnimationMode: UInt8, CustomStringConvertible {
    case on = 100
    case off = 101
    
    var description: String {
        switch self {
        case .on: return "On"
        case .off: return "Off"
        }
    }
}

enum CommandCode: UInt8 {
    case readSettings = 99
    case mode = 100
    case wakeUpTime = 101
    case sleepTime = 102
    case colorOne = 103
    case colorTwo = 104
    case colorThree = 105
    case colorFour = 106
    case colorFive = 107
    case animationMode = 108
    case time = 109
    case date = 110
}

struct RainbowCommand: Equatable {
    let code: CommandCode
    let arg1: UInt8?
    let arg2: UInt8?
    let arg3: UInt8?
    
    init(code: CommandCode, arg1: UInt8? = nil, arg2: UInt8? = nil, arg3: UInt8? = nil) {
        self.code = code
        self.arg1 = arg1
        self.arg2 = arg2
        self.arg3 = arg3
    }
    
    var payload: Data {
        var command: [UInt8] = [code.rawValue]
        
        command.append(arg1 ?? 0)
        command.append(arg2 ?? 0)
        command.append(arg3 ?? 0)
        
        return Data(bytes: command, count: command.count)
    }
}

enum ConnectionStatus: String {
    case idle, scanning, deviceFound, failed, disconnected, connected
}

class ValueTableViewCell: UITableViewCell {

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: .value1, reuseIdentifier: reuseIdentifier)
    }

    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

class ViewController: UITableViewController {
    let bleManager = BLE()
    
    var connectionStatus: ConnectionStatus = .idle
    var deviceMode: DeviceMode?
    var animationMode: AnimationMode?
    var wakeUpTime: Date?
    var sleepTime: Date?
    var colors: [UIColor]?

    override func viewDidLoad() {
        super.viewDidLoad()
        
        tableView.register(ValueTableViewCell.self, forCellReuseIdentifier: "Cell")
        
        bleManager.delegate = self
    }
    
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 10
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)

        switch indexPath.row {
        case 0:
            cell.textLabel?.text = "Connection Status"
            cell.detailTextLabel?.text = connectionStatus.rawValue
        case 1:
            cell.textLabel?.text = "Device Mode"
            
            if let deviceMode = deviceMode {
                cell.detailTextLabel?.text = "\(String(describing: deviceMode))"
            } else {
                cell.detailTextLabel?.text = "N/A"
            }
        case 2:
            cell.textLabel?.text = "Animation Mode"
            
            if let animationMode = animationMode {
                cell.detailTextLabel?.text = "\(String(describing: animationMode))"
            } else {
                cell.detailTextLabel?.text = "N/A"
            }
        case 3:
            cell.textLabel?.text = "Wake Up Time"
            
            if let wakeUpTime = wakeUpTime {
                cell.detailTextLabel?.text = "\(DateFormatter.localizedString(from: wakeUpTime, dateStyle: .none, timeStyle: .short))"
            } else {
                cell.detailTextLabel?.text = "N/A"
            }
        case 4:
            cell.textLabel?.text = "Sleep Time"
            
            if let sleepTime = sleepTime {
                cell.detailTextLabel?.text = "\(DateFormatter.localizedString(from: sleepTime, dateStyle: .none, timeStyle: .short))"
            } else {
                cell.detailTextLabel?.text = "N/A"
            }
        default:
            break
        }

        return cell
    }
}

extension ViewController : BLEDelegate {
    func bleDidDiscoverNewPeripheral(peripheral: CBPeripheral) {
        connectionStatus = .deviceFound
        
        if !bleManager.connectToPeripheral(peripheral: peripheral) {
            connectionStatus = .failed
        }

        tableView.reloadData()
    }
    
    func bleDidUpdateState() {
        if bleManager.centralManager?.state == .poweredOn {
            if (!bleManager.startScanning(timeout: 60)) {
                // TODO: Show alert
            }
        }
    }
    
    func bleDidConnectToPeripheral() {
        connectionStatus = .connected

        tableView.reloadData()
        
        let readCommand = RainbowCommand(code: .readSettings)
        
        bleManager.write(data: readCommand.payload)
    }
    
    func bleDidDisconnectFromPeripheral() {
        connectionStatus = .disconnected

        tableView.reloadData()
    }
    
    func bleDidReceiveData(data: Data?) {
        guard let data = data else {
            return
        }
        
        let bytes = [UInt8](data)
        
        print("Data received \(bytes)")
        
        for i in stride(from: 0, to: bytes.count, by: 4) {
            guard let commandCode = CommandCode(rawValue: bytes[i]) else {
                continue
            }
            
            let command = RainbowCommand(code: commandCode,
                                         arg1: bytes[i+1],
                                         arg2: bytes[i+2],
                                         arg3: bytes[i+3])
            
            switch commandCode {
            case .animationMode:
                if let arg1 = command.arg1 {
                    animationMode = AnimationMode(rawValue: arg1)
                }
            case .mode:
                if let arg1 = command.arg1 {
                    deviceMode = DeviceMode(rawValue: arg1)
                }
            case .wakeUpTime:
                if let hour = command.arg1, let min = command.arg2, let sec = command.arg3 {
                    wakeUpTime = Calendar.current.date(bySettingHour: Int(hour), minute: Int(min), second: Int(sec), of: Date())
                }
            case .sleepTime:
                if let hour = command.arg1, let min = command.arg2, let sec = command.arg3 {
                    sleepTime = Calendar.current.date(bySettingHour: Int(hour), minute: Int(min), second: Int(sec), of: Date())
                }
            default:
                break
            }
        }
        
        tableView.reloadData()
    }
}
