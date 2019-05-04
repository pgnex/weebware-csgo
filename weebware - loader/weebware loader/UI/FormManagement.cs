using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

public static class FormManagement
{
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    // === RANDOM STUFF ===

    public static void LabelButton(this Label label, Color hovered)
    {
        Color oColor = label.ForeColor;
        Cursor oCursor = label.Cursor;
        label.MouseEnter += (s, e) => {
            label.Cursor = Cursors.Hand;
            label.ForeColor = hovered;
        };
        label.MouseLeave += (s, e) => {
            label.Cursor = oCursor;
            label.ForeColor = oColor;
        };
    }

    // ==== MOVABLE COMPONENTS ===

    public const int WM_NCLBUTTONDOWN = 0xA1;
    public const int HT_CAPTION = 0x2;
    private static bool dragging = false;
    private static int xOffset = 0, yOffset = 0;

    private static void MouseUp() { dragging = false; }
    public static void Movable(this Form f, Label component) { component.MouseDown += (s, e) => MouseDown(f); component.MouseMove += (s, e) => MouseMove(f); component.MouseUp += (s, e) => MouseUp(); }
    public static void Movable(this Form f, Panel component) { component.MouseDown += (s, e) => MouseDown(f); component.MouseMove += (s, e) => MouseMove(f); component.MouseUp += (s, e) => MouseUp(); }
    public static void Movable(this Form f, PictureBox component) { component.MouseDown += (s, e) => MouseDown(f); component.MouseMove += (s, e) => MouseMove(f); component.MouseUp += (s, e) => MouseUp(); }

    private static void MouseDown(Form f)
    {
        dragging = true;
        xOffset = Cursor.Position.X - f.Location.X;
        yOffset = Cursor.Position.Y - f.Location.Y;
    }

    private static void MouseMove(Form f)
    {
        if (dragging)
        {
            f.Location = new Point(Cursor.Position.X - xOffset, Cursor.Position.Y - yOffset);
            f.Update();
        }
    }
}
