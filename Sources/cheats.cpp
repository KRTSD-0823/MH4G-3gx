#include "cheats.hpp"

#include "tools.hpp"

#include <sstream>
#include <vector>

namespace CTRPluginFramework
{

    /*
    メモ書き
    ・バイトの操作をする時、index(n番目)が1場合に右端、3が左端になる
    */

    using std::string;
    using std::to_string;
    using std::vector;

    using tools::GetStringVectorIndex;
    using tools::GetByte;
    using tools::SetByte;

    void SetItemAmount(MenuEntry *entry)
    {
        vector<vector<u32>> items;

        //アイテムが入る通常のポーチのデータの処理

        u32 items_address = 0x833CD8C;
        u32 items_last_address = 0x833CDE8;

        {
            u32 address = 0x0;
            int i;

            for (i = 0; address < items_last_address; i++)
            {
                u32 data = 0x0;

                address = items_address + i * 0x4;
                Process::Read32(address, data);

                /*MessageBox("デバッグ",
                    Utils::Format("address: [%X]", address)
                )();
                MessageBox("デバッグ",
                    Utils::Format("data: [%X]", data)
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 1): [%X]", GetByte(data, 1))
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 2): [%X]", GetByte(data, 2))
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 3): [%X]", GetByte(data, 3))
                )();*/

                items.push_back(vector<u32>());

                //アイテムID
                items[i].push_back(GetByte(data, 2));
                items[i].push_back(GetByte(data, 1));

                //個数
                items[i].push_back(GetByte(data, 3));
            }
        }

        //弾・ビンのポーチのデータの処理

        u32 bullets_address = 0x833CD6C;
        u32 bullets_last_address = 0x833CD88;

        {
            u32 address = 0x0;

            int offset = static_cast<int>(items.size());

            for (int i = 0; address < bullets_last_address; i++)
            {
                u32 data = 0x0;

                address = bullets_address + i * 0x4;
                Process::Read32(address, data);

                int index = offset + i;

                /*MessageBox("デバッグ",
                    Utils::Format("address: [%X]", address)
                )();
                MessageBox("デバッグ",
                    Utils::Format("data: [%X]", data)
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 1): [%X]", GetByte(data, 1))
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 2): [%X]", GetByte(data, 2))
                )();
                MessageBox("デバッグ",
                    Utils::Format("GetByte(data, 3): [%X]", GetByte(data, 3))
                )();
                MessageBox("デバッグ",
                    Utils::Format("i: [%d]", i)
                )();
                MessageBox("デバッグ",
                    Utils::Format("index: [%d]", index)
                )();*/

                items.push_back(vector<u32>());

                items[index].push_back(GetByte(data, 2));
                items[index].push_back(GetByte(data, 1));
                items[index].push_back(GetByte(data, 3));
            }
        }

        vector<string> choices;

        //選択したものがポーチの何番目なのかを調べる用
        vector<string> items_reference;

        int i = 0;

        //アイテムポーチの中身を選択肢に入れる
        for (vector<u32> data : items)
        {
            /*MessageBox("デバッグ",
                Utils::Format("data[1]: [%x]", data[1])
            )();
            MessageBox("デバッグ",
                Utils::Format("data[0]: [%x]", data[0])
            )();*/

            string text = "";

            //先にバイトを代入しておく
            u32 id = 0xFF00;
            string id_str = "";

            //アイテムIDへと合体させる
            SetByte(id, 1, data[1]);
            SetByte(id, 2, data[0]);

            /*MessageBox("デバッグ",
                Utils::Format("i: [%d]", i)
            )();
            MessageBox("デバッグ",
                Utils::Format("id: [%X]", id)
            )();*/

            std::ostringstream oss;

            //16進数の数値を文字列にする
            oss << std::hex << std::uppercase << id;
            id_str = oss.str();

            text += "ID:";
            text += id_str;
            text += ", i:";
            text += to_string(i + 1);
            text += ", 個数:";
            text += to_string(data[2]);

            items_reference.push_back(text);

            //IDがない場所(つまり空のポーチの場所)を除外する
            if (id == 0x0000)
            {
                i++;
                continue;
            }

            choices.push_back(text);

            i++;
        }

        string body = "どのアイテムの個数を変更しますか？\n";
        body += "ID:アイテムID\n";
        body += "i:ポーチの上から数えて何番目か\n";
        body += "個数:現在のアイテムの個数";

        Keyboard keyboard(body);
        keyboard.Populate(choices);
        keyboard.ChangeSelectedEntry(static_cast<int>(choices.size()));

        /*
        Populateを使って選択肢を作る場合は一度変数に代入してからifに入れる

        ・ダメな例
        こうしてしまうとvalueには入力結果が入るが、選択肢は表示されずに自由入力欄になる
        if (keyboard.Open(value) == 0)
        {
            ...
        }

        なので、返り値を一度変数に代入し、その値を条件に使う
        (型) value = keyboard.Open();
        if (value == 0)
        {
            ...
        }
        */

        //何番目の選択肢を選んだかを受け取る用
        int value = static_cast<int>(keyboard.Open());

        /*MessageBox("デバッグ",
            Utils::Format("value: [%d]", value)
        )();*/

        if (value >= 0)
        {
            Keyboard keyboard2("個数を入力してください。(0＜値＜99)");
            keyboard2.IsHexadecimal(false);
            keyboard2.SetMaxLength(2);

            //個数を受け取る用
            u8 value2 = -1;

            /*MessageBox("デバッグ",
                Utils::Format("value2: [%d]", value2)
            )();*/

            if (keyboard2.Open(value2) >= 0)
            {
                if (value2 < 0 || value2 > 99)
                {
                    MessageBox("エラー", "個数は0から99の範囲で入力してください。")();
                    return;
                }

                int index = GetStringVectorIndex(items_reference, choices[value]);

                u32 address = items_address + index * 0x4;

                if (address > items_last_address)
                {
                    /*
                    アイテムポーチの一つひとつの値は4つのアドレス？を使うため、
                    アイテムポーチの終点のアドレスから弾・ビンポーチの終点のアドレスを引いて
                    そこから÷4(アイテム1枠が使うアドレスの範囲)をすればどれくらい引けばいいのかが分かる。

                    なお、このようにするのはアイテムポーチの処理をした後に弾・ビンポーチの処理をしているからである。
                    アドレスの順番的には弾・ビンポーチが先に来る。
                    */

                    int difference = (items_last_address - bullets_last_address) / 0x4;

                    //弾・ビンポーチはゲーム内の順番的には先に来るが、ここでは最後に来るためここで調整
                    address = bullets_address + (index - difference) * 0x4;

                    /*MessageBox("デバッグ",
                        Utils::Format("difference: [%d]", difference)
                    )();
                    MessageBox("デバッグ",
                        Utils::Format("index: [%d]", index)
                    )();
                    MessageBox("デバッグ",
                        Utils::Format("address: [%X]", address)
                    )();*/
                }

                u32 item_value = 0xFF00FF;

                SetByte(item_value, 1, items[index][1]);
                SetByte(item_value, 2, items[index][0]);
                SetByte(item_value, 3, value2);

                Process::Write32(address, item_value);

                MessageBox("成功",
                    Utils::Format("[%d]", index + 1) +
                    "番目のアイテムを" +
                    Utils::Format("[%d]", value2) +
                    "個に変更しました。"
                )();
            } else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    //HPの上限を設定する
    void SetMAXHP(MenuEntry *entry)
    {
        u32 base;
        u32 address;

        Process::Read32(0x1062ED4, base);
        address = base + 0x1184;

        u32 HP;
        u8 HP_value;

        Process::Read32(address, HP);
        HP_value = GetByte(HP, 1);

        Keyboard keyboard("HPの上限値を入力してください。(0≦値≦10000)");
        keyboard.IsHexadecimal(false);
        keyboard.SetMaxLength(5);

        int value = keyboard.Open();

        if (value >= 0)
        {
            if (value < 0 || value > 10000)
            {
                MessageBox("エラー", "0から10000の範囲で入力してください。")();
                return;
            }

            SetByte(HP, 3, value);

            Process::Write32(address, HP);

            MessageBox("成功",
                Utils::Format("HPの上限値を[%d]", value) +
                "に変更しました。"
            )();
        }
    }

    //HP(緑)を設定する
    void SetGreenHP(MenuEntry *entry)
    {
        u32 base;
        u32 address;

        Process::Read32(0x1062ED4, base);
        address = base + 0x1184;

        u32 HP;
        u8 maxHP;

        Process::Read32(address, HP);
        maxHP = GetByte(HP, 3);

        Keyboard keyboard(
            Utils::Format("HP(緑)の値を入力してください。(0≦値≦[%d]", maxHP) +
            ")"
        );
        keyboard.IsHexadecimal(false);
        keyboard.SetMaxLength(3);

        int value = keyboard.Open();

        if (value >= 0)
        {
            if (value < 0 || value > maxHP)
            {
                MessageBox("エラー",
                    Utils::Format("0から[%d]", maxHP) +
                    "の範囲で入力してください。"
                )();
            }
            else
            {
                SetByte(HP, 1, value);

                /*MessageBox("デバッグ",
                    Utils::Format("HP[X]:[%X]", HP)
                )();

                MessageBox("デバッグ",
                    Utils::Format("maxHP[d]:[%d]", maxHP)
                )();

                MessageBox("デバッグ",
                    Utils::Format("maxHP[X]:[%X]", maxHP)
                )();*/

                Process::Write32(address, HP);

                MessageBox("成功",
                    Utils::Format("HP(緑)を[%d]", value) + "に変更しました。")();
            }
        }
    }

    //HP(赤)を設定する
    void SetRedHP(MenuEntry *entry)
    {
        u32 base;
        u32 address;

        u32 HP;
        u32 maxHP;

        Process::Read32(0x1062ED4, base);
        address = base + 0x1194;

        Process::Read32(address, HP);

        Process::Read32(address - 0x10, maxHP);
        maxHP = GetByte(maxHP, 3);

        Keyboard keyboard(
            Utils::Format("HP(赤)の値を入力してください。(0≦値≦[%d]", maxHP) +
            ")"
        );
        keyboard.IsHexadecimal(false);
        keyboard.SetMaxLength(3);

        int value = keyboard.Open();

        if (value >= 0)
        {
            if (value < 0 || value > maxHP)
            {
                MessageBox("エラー",
                    Utils::Format("0から[%d]", maxHP) +
                    "の範囲で入力してください。"
                )();
            }
            else
            {
                SetByte(HP, 1, value);

                Process::Write32(address, HP);

                MessageBox("成功",
                    Utils::Format("HP(赤)を[%d]", value) +
                    "に変更しました。"
                )();
            }
        }
    }

    //HPを固定するチート

    bool FixHP_activated = false;

    void FixHP(MenuEntry *entry)
    {
        u32 base;
        u32 GreenHP_address;
        u32 RedHP_address;

        Process::Read32(0x1062ED4, base);
        GreenHP_address = base + 0x1184;
        RedHP_address = base + 0x1194;

        u32 GreenHP;
        u32 RedHP;

        //オンになった時にのみ動作させる処理
        if (entry->IsActivated())
        {
            //オンにした時、一度だけ実行する処理
            if (!FixHP_activated)
            {
                Process::Read32(GreenHP_address, GreenHP);
                Process::Read32(RedHP_address, RedHP);

                string body = "";

                body += "現在のHPを超えるダメージを受けた際には力尽きてしまいます。ご注意ください。\n";
                body += "また、このチートを有効にしている間は秘薬などによる体力上限UPの効果が得られません。";

                MessageBox("注意", body)();
            }

            FixHP_activated = true;
        }
        else
        {
            FixHP_activated = false;
        }

        Process::Write32(GreenHP_address, GreenHP);
        Process::Write32(RedHP_address, RedHP);
    }
}