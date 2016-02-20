//
//  ViewController.swift
//  PeltierControl
//
//  Created by Rohit Banda on 2/19/16.
//  Copyright © 2016 Rohit Banda. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    @IBOutlet weak var peltierTemo: UILabel!
    @IBOutlet weak var outsideTemp: UILabel!
    @IBOutlet weak var tempOverride: UISwitch!

    @IBOutlet weak var overrideBox: UITextField!

    @IBOutlet weak var overrideSlider: UISlider!
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

