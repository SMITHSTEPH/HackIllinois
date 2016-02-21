//
//  ViewController.swift
//  PeltierControl
//
//  Created by Rohit Banda on 2/19/16.
//  Copyright © 2016 Rohit Banda. All rights reserved.
//

import UIKit
import Alamofire


class ViewController: UIViewController {
    @IBOutlet weak var peltierTemo: UILabel!
    @IBOutlet weak var outsideTemp: UILabel!
    @IBOutlet weak var tempOverride: UISwitch!
    @IBOutlet weak var overrideTemp: UILabel!
    @IBOutlet weak var uiSliderOv: UISlider!
    @IBOutlet weak var setButton: UIButton!
    
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tempOverride.setOn(false, animated: true)
        overrideTemp.hidden = true
        uiSliderOv.hidden = true
        setButton.hidden = true
        peltierTemo.text = " "
        outsideTemp.text = " "
        overrideTemp.text = "\(uiSliderOv.value)"
        refreshStuff()
        refreshStuff2()
        NSTimer.scheduledTimerWithTimeInterval(600.0, target: self, selector: Selector("refreshStuff"), userInfo: nil, repeats: true)
        NSTimer.scheduledTimerWithTimeInterval(3.0, target: self, selector: Selector("refreshStuff2"), userInfo: nil, repeats: true)
    }
    
    func postStuff(input1: Int){
        let newPost = ["target": input1]
        Alamofire.request(.POST, "http://192.168.1.130", parameters: newPost, encoding: .JSON)
        
    }
    func refreshStuff2(){
        Alamofire.request(.GET, "http://192.168.1.130")
            .response { (_, _, data, error) in
                let str = NSString(data: data!, encoding: NSUTF8StringEncoding)
                print(str)
                dispatch_async(dispatch_get_main_queue()) {
                    self.peltierTemo.text = "\(str!)" + "\u{00B0}F"
                }
                
        }
        
    }
    func refreshStuff() {
        Alamofire.request(.POST, "http://api.openweathermap.org/data/2.5/weather?q=Urbana%20Champaign&appid=44db6a862fba0b067b1930da0d769e98", encoding:.JSON).responseJSON
            { response in switch response.result {
            case .Success(let JSON):
                //                print("Success with JSON: \(JSON)")
                
                let response = JSON as! NSDictionary
                
                //example if there is an id
                let main = response.objectForKey("main")!
                let temp = main["temp"]
                dispatch_async(dispatch_get_main_queue()) {
                    self.outsideTemp.text = "\(round((Double("\(temp!!)")! * 1.8)-459.67))" + "\u{00B0}F"
                }
                
            case .Failure(let error):
                print("Request failed with error: \(error)")
                }
        }
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    @IBAction func manualOvveride(sender: AnyObject) {
        if(tempOverride.on) {
            overrideTemp.hidden = false
            uiSliderOv.hidden = false
            setButton.hidden = false
        }
        else {
            overrideTemp.hidden = true
            uiSliderOv.hidden = true
            setButton.hidden = true
        }
    }
    
    @IBAction func setButtonPress(sender: AnyObject) {
        postStuff(Int(uiSliderOv.value))
    }
    @IBAction func uiSliderC(sender: UISlider) {
        overrideTemp.text = "\(Int(uiSliderOv.value))"
    }
    
}
