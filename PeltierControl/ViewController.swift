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
   
    

    
    override func viewDidLoad() {
        super.viewDidLoad()
        tempOverride.setOn(false, animated: true)
        overrideTemp.hidden = true
        uiSliderOv.hidden = true
        //peltierTemo.text = " "
        outsideTemp.text = " "
        overrideTemp.text = "\(uiSliderOv.value)"
        NSTimer.scheduledTimerWithTimeInterval(5.0, target: self, selector: Selector("refreshStuff"), userInfo: nil, repeats: true)
        // Do any additional setup after loading the view, typically from a nib.
    }
    func refreshStuff() {
        let urlPath: String = "192.168.1.100"
        let url: NSURL = NSURL(string: urlPath)!
        let request1: NSMutableURLRequest = NSMutableURLRequest(URL: url)
        
        request1.HTTPMethod = "GET"
        //let stringPost="deviceToken=123456" // Key and Value
        
        //let data = stringPost.dataUsingEncoding(NSUTF8StringEncoding)
        
        request1.timeoutInterval = 60
//        request1.HTTPBody=data
        request1.HTTPShouldHandleCookies=false
        
        
        let task = NSURLSession.sharedSession().dataTaskWithRequest(request1) {(data, response, error) in
            print(NSString(data: data!, encoding: NSUTF8StringEncoding))
            dispatch_async(dispatch_get_main_queue()) {
                self.peltierTemo.text = "hi"
                
            }
        }
        
        task.resume()

    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    @IBAction func manualOvveride(sender: AnyObject) {
        if(tempOverride.on) {
            overrideTemp.hidden = false
            uiSliderOv.hidden = false
        }
        else {
            overrideTemp.hidden = true
            uiSliderOv.hidden = true
        }
    }
    
    @IBAction func uiSliderC(sender: UISlider) {
        overrideTemp.text = "\(Int(uiSliderOv.value))"
    }

}
